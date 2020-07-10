// modelo bruto

#include <ilcplex/ilocplex.h>
#include <sys/types.h> 
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

ILOSTLBEGIN

int index1(int j, int i, int n){
    i = (int) j*n+i;
    return i;
}

int index2(int j, int i, int d){
    i = (int) j*d+i;
    return i;
}

//==========================================================
// modelo com menos variáveis
//==========================================================

void MDMWNPP(int n, int k, int d, char tipo){
    IloEnv env;
    try {
    	//==================================================
    	// carrega a instância
    	//==================================================

		//int n, k, d;

		FILE *arquivo;
		char inst[30];

		int j, j1, j2, i, l; //l coordenada

		sprintf(inst, "inst/mdtwnpp_500_20%c.txt", tipo);
		arquivo = fopen(inst,"r");

		double v[n][d];

		fscanf(arquivo, "%d\t%d\n",&i,&j);

		for(i=0; i<n; i++){
			for(l=0; l<d; l++){
				fscanf(arquivo, "%lf\t", &v[i][l]);
			}
			fscanf(arquivo, "\n");
		}

		fclose(arquivo);
		
		int N = n*k;

		//=======================================================
		// define variáveis
		//=======================================================

		IloNumVarArray x(env, N, 0, 1, ILOBOOL);
		IloNumVar y(env, 0, IloInfinity, ILOFLOAT);

		//=========================================================
		// cria o modelo
		//=========================================================

		IloModel modelo(env);

		modelo.add(IloMinimize(env, y));
		
		for(j1=0;j1<k-1;j1++){
			for(j2=j1+1; j2<k; j2++){
				for(l=0; l<d; l++){
					IloExpr Expr1(env);
					for(i=0;i<n;i++){
						Expr1 = Expr1 + v[i][l]*(x[index1(j1,i,n)]-x[index1(j2,i,n)]);
					}
					modelo.add(Expr1 - y <= 0);
					modelo.add(Expr1 + y >= 0);
					Expr1.end();
				}
			}		
		}

		for(i=0;i<n;i++){
			IloExpr Expr2(env);
			for(j=0;j<k;j++){
				Expr2 = Expr2 + x[index1(j,i,n)];
			}
			modelo.add(Expr2 == 1);
			Expr2.end();
		}

		for(j=0;j<k;j++){
			IloExpr Expr3(env);
			for(i=0;i<n;i++){
				Expr3 = Expr3 + x[index1(j,i,n)];
			}
			modelo.add(Expr3 >= 1);
			Expr3.end();
		}
		
		//================================================================
		// parametros do solver
		//================================================================
		
		IloCplex cplex(modelo);

		cplex.setOut(env.getNullStream());
		cplex.setWarning(env.getNullStream());
		cplex.setParam(IloCplex::Param::TimeLimit, 1800); //tempo limite (segundos)
    	cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory, 4096); //memória limite (MB)
    	cplex.setParam(IloCplex::Param::Threads, 1); // uma thread
    	
        cplex.solve();
        
        sprintf(inst, "%d_%d_%d%c", k, n, d, tipo); 
        env.out()  << inst << "\t" << cplex.getStatus() << "\t" << cplex.getObjValue() << "\t"<< cplex.getBestObjValue() << "\t" << env.getTime() << endl;

        cplex.end(); //método de solução
		modelo.end(); //modelo matemático
		env.end(); // ambiente
    } catch (IloException& ex) {
        cerr << "Erro Cplex: " << ex << endl;
    } catch (...) {
        cerr << "Erro Cpp:" << endl;
    }
    
}

int main(int argc, char* argv[]){
	
	char tipo[] = {'a', 'b', 'c', 'd', 'e'};
	int k = atof(argv[1]);
	int n[] = {50, 100, 200, 300, 400, 500};
	int d[] = {2, 3, 4, 5, 10, 15, 20};
	
	for(int itipo=0; itipo<5; itipo++){
		for(int in=0; in<6; in++){
			for(int id=0; id<7; id++){
				MDMWNPP(n[in], k, d[id], tipo[itipo]);
			}
		}
	}
	
	/*
	int n=50;
	int k=2;
	int d=3;
	char tipo='a';

	MDMWNPP(n,k,d,tipo);
	*/
 	
 	return 0;
 	
}