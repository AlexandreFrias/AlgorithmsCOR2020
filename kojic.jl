using JuMP
using Cbc

function SolveProblem1(A, n::Int64, d::Int64)

	m=Model(solver=CbcSolver())

	@variable(m, x[1:n], Bin)
	@variable(m, y>=0)


    for l=1:d
        @constraint(m, -0.5*y + sum(A[i,l]*x[i] for i=1:n) <= 0.5*sum(A[i,l] for i=1:n))
        @constraint(m, 0.5*y + sum(A[i,l]*x[i] for i=1:n) >= 0.5*sum(A[i,l] for i=1:n))
    end

    #@constraint(m, sum(x[i] for i=1:n)>=1) # these two constraints enforce non-empty subsets
    #@constraint(m, sum(x[i] for i=1:n)<=n-1)

	@objective(m, Min, y)

	println(m)

    t1 = @elapsed status = solve(m)

    println(t1)

    println("Objective value: ", getobjectivevalue(m))
	println("x = ", getvalue(x))

end



#n=10; d=3; A=[141 484 2376; 874 615 4884; 760 140 2383; 248 254 3387; 8471 133 493; 25514 48 785; 561 1452 730; 55 2182 2055; 164 132 8686; 7491 553 315]
#n=10; d=3; A=[14 48 23; 87 61 48; 76 14 23; 24 25 33; 84 13 49; 25 48 78; 56 14 73; 55 21 20; 16 13 86; 74 55 31]
#n=5; d=2; A=[1 3; 4 4; 3 -2; 2 5; 2 -1]
#n=4; d=2; A=[1 3; 5 5; 3 -2; -3 12]

#n=4; d=4; A=[2 0 0 0; 0 3 0 0; 0 0 6 0; 0 0 0 1]
n=4; d=2; A=[3 3; -1 -1; -1 -1; -1 -1]
#n=6; d=2; A=[1 3; -1 -2; 0 -1; 6 5; -3 -3; -3 -2]

SolveProblem1(A, n, d)

# Min y
# Subject to
#  -0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≤ 0
#  0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≥ 0
#  -0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≤ 0
#  0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≥ 0
#  x[1] + x[2] + x[3] + x[4] ≥ 1
#  x[1] + x[2] + x[3] + x[4] ≤ 3
#  x[i] ∈ {0,1} ∀ i ∈ {1,2,3,4}
#  y ≥ 0

# 0.803944244
# Objective value: 2.0
# x = [0.0,0.0,0.0,1.0]

# alexandre@alexandre-Inspiron-5567:~/Documentos/MDMWNPP_MIP/teste_kojic$ julia kojic.jl
# Min y
# Subject to
#  -0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≤ 0
#  0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≥ 0
#  -0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≤ 0
#  0.5 y + 3 x[1] - x[2] - x[3] - x[4] ≥ 0
#  x[i] ∈ {0,1} ∀ i ∈ {1,2,3,4}
#  y ≥ 0

# 0.804733136
# Objective value: 0.0
# x = [0.0,0.0,0.0,0.0]
