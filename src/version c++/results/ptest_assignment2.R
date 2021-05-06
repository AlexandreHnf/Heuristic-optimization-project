new.getPvalues <- function(aa, bb, myfile) {
  a.cost <- myfile[,aa]
  b.cost <- myfile[,bb]
  tt_pvalue <- t.test(a.cost, b.cost, paired=T, conf.level = 0.99)$p.value
  print(paste("t-test p-value : ", tt_pvalue))
  wt_pvalue <- wilcox.test(a.cost, b.cost, paired=T, conf.level = 0.99)$p.value
  print(paste("wilcoxon test p-value : ", wt_pvalue))
  
  pvalues <- c(tt_pvalue, wt_pvalue)
  return(pvalues)
}

# open files 
myfile50 <- read.table("SLS_avgrunsRPDs_50.csv", header=TRUE, sep=",")
myfile100 <- read.table("SLS_avgrunsRPDs_100.csv", header=TRUE, sep=",")

p <- new.getPvalues("GA", "TS", myfile50)
p <- new.getPvalues("GA", "TS", myfile100)
