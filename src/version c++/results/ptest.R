new.function <- function(a, b) {
  best.known <- read.csv("bestSolutions50.txt", header=TRUE)
  myfile <- read.table("all_algos_all_instances_wct.txt", header=TRUE, sep=",")
  a.cost <- myfile[, a]
  a.cost <- 100 * (a.cost - best.known$BS) / best.known$BS
  b.cost <- myfile[, b]
  b.cost <- 100 * (b.cost - best.known$BS) / best.known$BS
  print("p-value : ")
  print(t.test(a.cost, b.cost, paired=T)$p.value)
  print("wilcow test : ")
  print(wilcox.test(a.cost, b.cost, paired=T)$p.value)
}


print("R_FI_T & R_FI_E")
new.function(2, 3)

print("1) which initial solution is preferable ? ")

# R-FI-I & S-FI-I
# R-FI-E & S-FI-E
# R-FI-T & S-FI-T
# R-BI-I & S-BI-I
# R-BI-E & S-BI-E
# R-BI-T & S-BI-T

print("2) which pivoting rule generates better quality solutions ? ")

# FI-R-I & BI-R-I
# FI-R-E & BI-R-E
# FI-R-T & BI-R-T
# FI-S-I & BI-S-I
# FI-S-E & BI-S-E
# FI-S-T & BI-S-T

print("4) which neighborhood generates better quality solutions ? ")

# I-R-FI & E-R-FI
# I-R-BI & E-R-BI
# I-S-FI & E-S-FI
# I-S-BI & E-S-BI

# I-R-FI & T-R-FI
# I-R-BI & T-R-BI
# I-S-FI & T-S-FI
# I-S-BI & T-S-BI

# T-R-FI & E-R-FI
# T-R-BI & E-R-BI
# T-S-FI & E-S-FI
# T-S-BI & E-S-BI