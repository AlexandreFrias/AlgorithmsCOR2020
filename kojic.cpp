
#include <ilcplex/ilocplex.h>
#include <sys/types.h> 
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
ILOSTLBEGIN

void libera_matriz(double **matriz, int lin){
	int i;
	for(i = 0; i < lin; i++){
		free(matriz[i]);
	}
	free(matriz);

	matriz=NULL;
}

double **leitura(char tipo){

	
	FILE *arquivo;
	char inst[30];

	sprintf(inst, "inst/mdtwnpp_500_20%c.txt", tipo);
	arquivo = fopen(inst,"r");

	double **v;
	int i,l,n,d;

	fscanf(arquivo, "%d\t%d\n",&n,&d);

	v = (double **)malloc (n*sizeof (double *));
	for ( i = 0; i < n; i++){
		v[i] = (double *)malloc(d*sizeof (double));
	}

	for(i=0; i<n; i++){
		for(l=0; l<d; l++){
			fscanf(arquivo, "%lf\t", &v[i][l]);
		}
		fscanf(arquivo, "\n");
	}
	fclose(arquivo);

	return v;
}



void MDTWNPP(double **v, int n, int d, char tipo){
    IloEnv env;
    try {
    	//==================================================
    	// instance data and indexes
    	//==================================================
		char inst[30];
		sprintf(inst, "%d_%d_%d%c", 2, n, d, tipo); 

		int i, l; //l coordenada

		double soma[d];

		for(l=0;l<d; l++){
			soma[l]=0;
		}


		for(i=0; i<n; i++){
			for(l=0; l<d; l++){
				soma[l]+=v[i][l];
			}
		}

		//=======================================================
		// define variáveis
		//=======================================================

		IloNumVarArray x(env, n, 0, 1, ILOBOOL);
		IloNumVar y(env, 0, IloInfinity, ILOFLOAT);

		//=========================================================
		// cria o modelo
		//=========================================================

		IloModel modelo(env);

		modelo.add(IloMinimize(env, y));
		
		for(l=0; l<d; l++){
			IloExpr Expr1(env);
			for(i=0; i<n; i++){
				Expr1 = Expr1 + v[i][l]*x[i];
			}
			modelo.add(Expr1 - y*0.5 <= soma[l]*0.5);
			modelo.add(Expr1 + y*0.5 >= soma[l]*0.5);
			Expr1.end();
		}
		
		//================================================================
		// parametros do solver
		//================================================================
		
		IloCplex cplex(modelo);

		cplex.setOut(env.getNullStream());
		cplex.setWarning(env.getNullStream());
		cplex.setParam(IloCplex::Param::TimeLimit, 1800); // tempo limite
    	cplex.setParam(IloCplex::Param::MIP::Limits::TreeMemory, 4096); //memória máxima
    	cplex.setParam(IloCplex::Param::Threads, 1); //uma thread
        
        cplex.solve();
        
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
	int n[]={50, 100, 200, 300, 400, 500};
	int d[]={2, 3, 4, 5, 10, 15, 20};
	double **v;
	for(int itipo=0; itipo<5; itipo++){
		v=leitura(tipo[itipo]);
		for(int in=0; in<6; in++){
			for(int id=0; id<7; id++){
				MDTWNPP(v, n[in], d[id], tipo[itipo]);
			}
		}
		libera_matriz(v, 500); //all v has the size 500
	}
	
	/*
	int n=50;
	int d=3;
	char tipo='a';
	double **v=leitura(tipo);

	MDTWNPP(v,n,d,tipo);
	*/
 	
 	return 0;
 	
}
