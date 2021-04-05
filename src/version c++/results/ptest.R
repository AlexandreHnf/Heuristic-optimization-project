new.getPvalues <- function(aa, bb) {
  best.known <- read.csv("bestSolutions50.txt", header=TRUE)
  myfile <- read.table("II_WCT_50.txt", header=TRUE, sep=",")
  a.cost <- myfile[,aa] 
  a.cost <- 100 * (a.cost - best.known$BS) / best.known$BS
  b.cost <- myfile[,bb] 
  b.cost <- 100 * (b.cost - best.known$BS) / best.known$BS
  tt_pvalue <- t.test(a.cost, b.cost, paired=T)$p.value
  print(paste("t-test p-value : ", tt_pvalue))
  wt_pvalue <- wilcox.test(a.cost, b.cost, paired=T)$p.value
  print(paste("wilcow test p-value : ", wt_pvalue))
  
  pvalues <- c(tt_pvalue, wt_pvalue)
  return(pvalues)
}

new.getTTpvalues <- function(aa, bb, best_knowns, res_file) {
  acost = res_file[,aa]
  acost <- 100 * (acost - best.known$BS) / best.known$BS
  bcost <- myfile[,bb] 
  bcost <- 100 * (bcost - best.known$BS) / best.known$BS
  tt_pvalue <- t.test(acost, bcost, paired=T)$p.value
  print(paste("t-test p-value : ", tt_pvalue))
  return(tt_pvalue)
}

new.getWTpvalues <- function(aa, bb, best_knowns, res_file) {
  acost = res_file[,aa]
  acost <- 100 * (acost - best.known$BS) / best.known$BS
  bcost <- myfile[,bb] 
  bcost <- 100 * (bcost - best.known$BS) / best.known$BS
  tt_pvalue <- t.test(acost, bcost, paired=T)$p.value
  wt_pvalue <- wilcox.test(a.cost, b.cost, paired=T)$p.value
  print(paste("wilcow test p-value : ", wt_pvalue))
  return(wt_pvalue)
}

new.allCombiPvalues <- function(algo_names) {
  names <- c() # all algo combinations names
  all_pvalues <- matrix(NA, nrow=132, ncol=2) # all p values ttest + wilcox-test for all combi
  
  nb_it <- 0
  for (ai in algo_names) {
    for (aj in algo_names) {
      name <- paste(ai, ",", aj)
      print(name)
      if (ai != aj) {
        names[nb_it + 1] <- name    # append algo combination name
        p <- new.getPvalues(ai, aj) # p = (ttest p, wilcoxtest p)
        print(p[1])
        print(p[2])
        all_pvalues[nb_it + 1, ] <- p # append row (ttest p, wilcoxtest p)
        nb_it = nb_it + 1
      }
    }
  }
  
  print(paste("nb of combi : ", nb_it))
  rownames(all_pvalues) <- names
  #print(all_pvalues$`S_BI_I , S_BI_E`)
  print(all_pvalues[1,])
  return(all_pvalues)
}

# open files 
best.known <- read.csv("bestSolutions50.txt", header=TRUE)
myfile <- read.table("II_WCT_50.txt", header=TRUE, sep=",")

algo_names <- c("R_FI_T","R_FI_E","R_FI_I","R_BI_T","R_BI_E","R_BI_I","S_FI_T","S_FI_E","S_FI_I","S_BI_T","S_BI_E","S_BI_I")

# simple test
print("R_FI_T & R_FI_E")
p <- new.getPvalues("R_FI_T", "R_FI_E")

# get all pvalues
all_pvalues <- new.allCombiPvalues(algo_names)
print(all_pvalues)


# write pvalues to file
write.table(all_pvalues, file="pvalues.txt", sep="\t", col.names = F, row.names = F)

# question 1
