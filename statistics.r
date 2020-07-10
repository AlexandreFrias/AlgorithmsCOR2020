library(xtable)

#==========================================================
#Script to create results
#----------------------------------------------------------

print('==========================================================================')
print('Descriptive Analysis')
print('==========================================================================')

#Leitura de dados
result0 = read.csv("result0.csv", header = FALSE, sep = "\t")
result1 = read.csv("result1.csv", header = FALSE, sep = "\t")
result2 = read.csv("result2.csv", header = FALSE, sep = "\t")
result3 = read.csv("result3.csv", header = FALSE, sep = "\t")

#Escolhendo nomes das colunas
colnames(result0) = c("Instance", "Status", "UB", "LB", "Time")
colnames(result1) = c("Instance", "Status", "UB", "LB", "Time")
colnames(result2) = c("Instance", "Status", "UB", "LB", "Time")
colnames(result3) = c("Instance", "Status", "UB", "LB", "Time")

#Removing names of instances
result0$Instance = NULL
result1$Instance = NULL
result2$Instance = NULL
result3$Instance = NULL

#Printing descriptive tables
summary(result0)
summary(result1)
summary(result2)
summary(result3)

print(xtable(summary(result0)))

boxplot(result0$UB, result3$UB)
boxplot(result0$LB, result3$LB)
boxplot(result0$Time, result3$Time)

print('==========================================================================')
print(' t-test and its assumptions')
print('==========================================================================')

#Normality test
shapiro.test(result0$UB)
shapiro.test(result0$LB)
shapiro.test(result0$Time)

shapiro.test(result3$UB)
shapiro.test(result3$LB)
shapiro.test(result3$Time)

#verify the equality of variance
var.test(result0$UB, result3$UB, alternative = "two.sided")
var.test(result0$LB, result3$LB, alternative = "two.sided")
var.test(result0$Time, result3$Time, alternative = "two.sided") #without equality

#Wilcoxon test
wilcox.test(result0$UB, result3$UB, alternative =  "less", paired = TRUE, var.equal = TRUE, conf.level = 0.95)
wilcox.test(result0$LB, result3$LB, alternative =  "greater", paired = TRUE, var.equal = TRUE, conf.level = 0.95)
wilcox.test(result0$Time, result3$Time, alternative =  "two.sided", paired = TRUE, var.equal = FALSE, conf.level = 0.95)

