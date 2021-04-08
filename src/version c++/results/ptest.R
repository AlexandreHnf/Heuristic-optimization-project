new.getPvalues <- function(aa, bb, best_knowns, myfile) {
  #best.known <- read.csv("bestSolutions.txt", header=TRUE)
  #myfile <- read.table("II_WCT_test_inv.txt", header=TRUE, sep=",")
  a.cost <- myfile[,aa] 
  a.cost <- 100 * (a.cost - best.known$BS) / best.known$BS
  b.cost <- myfile[,bb] 
  b.cost <- 100 * (b.cost - best.known$BS) / best.known$BS
  tt_pvalue <- t.test(a.cost, b.cost, paired=T, conf.level = 0.99)$p.value
  #print(paste("t-test p-value : ", tt_pvalue))
  wt_pvalue <- wilcox.test(a.cost, b.cost, paired=T, conf.level = 0.99)$p.value
  #print(paste("wilcow test p-value : ", wt_pvalue))
  
  pvalues <- c(tt_pvalue, wt_pvalue)
  return(pvalues)
}

new.allCombiPvalues <- function(algo_names, nb_combinations, best_knowns, myfile) {
  names <- c() # all algo combinations names
  all_pvalues <- matrix(NA, nrow=nb_combinations, ncol=2) # all p values ttest + wilcox-test for all combi
  
  nb_it <- 0
  for (ai in algo_names) {
    for (aj in algo_names) {
      name <- paste(ai, ",", aj)
      #print(name)
      if (ai != aj) {
        names[nb_it + 1] <- name    # append algo combination name
        p <- new.getPvalues(ai, aj, best_knowns, myfile) # p = (ttest p, wilcoxtest p)
        all_pvalues[nb_it + 1, ] <- c(p[1], p[2]) # append row (ttest p, wilcoxtest p)
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

new.printBestPvalues<- function(all_pvalues, alpha) {
  n <- rownames(all_pvalues) # row names
  print("BEST PVALUES:")
  for (i in 1:length(all_pvalues[, 1])) {
    #print(paste("===> ", all_pvalues[i,]))
    if (all_pvalues[i,][1] > alpha) {
      print(n[i]) # print row name (pair of algos)
      print(all_pvalues[i,]) 
    }
  }
}

# ================================ TEST pvalues ==============================================
# ==============================================================================================
best.known <- read.csv("bestSolutions.txt", header=TRUE)
myfile <- read.table("II_WCT_test_inv.txt", header=TRUE, sep=",")
p <- new.getPvalues("RFI", "SFI", best_knowns, myfile)
print(p[1])
print(p[2])

# ================================ TEST ITERATIVE IMPROVEMENT ==================================
# ==============================================================================================
# open files 

myfileII <- read.table("II_WCT_test_inv.txt", header=TRUE, sep=",")

algo_names_II <- c("RFT","RFE","RFI","RBT","RBE","RBI","SFT","SFE","SFI","SBT","SBE","SBI")

# get all pvalues
all_pvalues_II <- new.allCombiPvalues(algo_names_II, 132, best.knowns, myfileII) # 132 combinations with 12 algos
print(all_pvalues_II)

# print all best pvalues (above alpha = 0.05), which means pairs of similar algorithms
new.printBestPvalues(all_pvalues_II, 0.01) 

# write pvalues to file
write.table(all_pvalues_II, file="pvalues_II.txt", sep="\t", col.names = F, row.names = F)

# question 1
print(paste("RFI-SFI :", all_pvalues_II["RFI , SFI",]))
print(paste("RFE-SFE :", all_pvalues_II["RFE , SFE",]))
print(paste("RFT-SFT :", all_pvalues_II["RFT , SFT",]))
print(paste("RBI-SBI :", all_pvalues_II["RBI , SBI",]))
print(paste("RBE-SBE :", all_pvalues_II["RBE , SBE",]))
print(paste("RBT-SBT :", all_pvalues_II["RBT , SBT",]))

# question 2 
print(paste("RFI-RBI :", all_pvalues_II["RFI , RBI",]))
print(paste("RFE-RBE :", all_pvalues_II["RFE , RBE",]))
print(paste("RFT-RBT :", all_pvalues_II["RFT , RBT",]))
print(paste("SFI-SBI :", all_pvalues_II["SFI , SBI",]))
print(paste("SFE-SBE :", all_pvalues_II["SFE , SBE",]))
print(paste("SFT-SBT :", all_pvalues_II["SFT , SBT",]))

# question 3 
print(paste("RFI-RFE :", all_pvalues_II["RFI , RFE",]))
print(paste("RBI-RBE :", all_pvalues_II["RBI , RBE",]))
print(paste("SFI-SFE :", all_pvalues_II["SFI , SFE",]))
print(paste("SBI-SBE :", all_pvalues_II["SBI , SBE",]))

print(paste("RFI-RFT :", all_pvalues_II["RFI , RFT",]))
print(paste("RBI-RBT :", all_pvalues_II["RBI , RBT",]))
print(paste("SFI-SFT :", all_pvalues_II["SFI , SFT",]))
print(paste("SBI-SBT :", all_pvalues_II["SBI , SBT",]))

print(paste("RFT-RFE :", all_pvalues_II["RFT , RFE",]))
print(paste("RBT-RBE :", all_pvalues_II["RBT , RBE",]))
print(paste("SFT-SFE :", all_pvalues_II["SFT , SFE",]))
print(paste("SBT-SBE :", all_pvalues_II["SBT , SBE",]))


# ============================== TEST VARIABLE NEIGHBOURHOOD DESCENT ====================================
# ==============================================================================================
# open file
myfileVND <- read.table("VND_WCT_test_inv.txt", header=TRUE, sep=",")

algo_names_VND <- c("RTEI","RTIE","STEI","STIE")

# get all pvalues
all_pvalues_VND <- new.allCombiPvalues(algo_names_VND, 12, best.knowns, myfileVND) # 12 combinations with 4 algos
print(all_pvalues_VND)

# print all best pvalues (above alpha = 0.05), which means pairs of similar algorithms
new.printBestPvalues(all_pvalues_VND, 0.01) 

# write pvalues to file
write.table(all_pvalues_VND, file="pvalues_VND.txt", sep="\t", col.names = F, row.names = F)

# pvalues comparisons : 
print(paste("RTEI-RTIE :", all_pvalues_VND["RTEI , RTIE",]))
print(paste("RTEI-STEI :", all_pvalues_VND["RTEI , STEI",]))
print(paste("RTEI-STIE :", all_pvalues_VND["RTEI , STIE",]))
print(paste("RTIE-STEI :", all_pvalues_VND["RTIE , STEI",]))
print(paste("RTIE-STIE :", all_pvalues_VND["RTIE , STIE",]))
print(paste("STEI-STIE :", all_pvalues_VND["STEI , STIE",]))