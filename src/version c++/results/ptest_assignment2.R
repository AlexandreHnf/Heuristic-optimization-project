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

new.getSpearmanPvalue <- function(aa, bb, myfile) {
  a.cost <- myfile[,aa]
  b.cost <- myfile[,bb]
  spearman <- cor.test(a.cost, b.cost, method = "spearman")
  print(paste("spearman test : ", spearman))
}

####################################################################################

# open files 
myfile50 <- read.table("SLS_avgrunsRPDs_50.csv", header=TRUE, sep=",")
myfile100 <- read.table("SLS_avgrunsRPDs_100.csv", header=TRUE, sep=",")


all_pvalues <- matrix(NA, nrow=2, ncol=2) # all p values ttest + wilcox-test for 50 and 100 jobs

p <- new.getPvalues("GA", "TS", myfile50)
all_pvalues[1, ] <- c(p[1], p[2]) # append row (ttest p, wilcoxtest p)
p <- new.getPvalues("GA", "TS", myfile100)
all_pvalues[2, ] <- c(p[1], p[2]) # append row (ttest p, wilcoxtest p)

# write pvalues to file
write.table(all_pvalues, file="pvalues_SLS.csv", sep=",", col.names = F)

####################################################################################

sp <- new.getSpearmanPvalue("GA", "TS", myfile50)
sp <- new.getSpearmanPvalue("GA", "TS", myfile100)

