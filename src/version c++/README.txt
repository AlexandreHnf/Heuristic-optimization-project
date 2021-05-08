./flowshopRUN II <init> (R, SRZ) <pivoting> (FI, BI) <neighbour> (T, E, I) <instance_file>
./flowshopRUN VND <init> (R, SRZ) <n1> <n2> <n3> (T E I or T I E) <instance_file>"
./flowshopRUN <runAllExperimentsA1>
./flowshopRUN <TS> <tenure> <max_time> <instance_file>
./flowshopRUN <GA> <Pm> <Pc> <Pe> <COUNT> <pop size> <max_time> <instance_file>
./flowshopRUN <runAllExperimentsA2>

e.g. ./flowshopRUN II R FI T instances/50_20_01
e.g. ./flowshopRUN VND SRZ T E I instances/100_20_01
e.g. ./flowshopRUN runAllExperiments
e.g. ./flowshopRUN TS 7 150.0 instances/50_20_01
e.g. ./flowshopRUN GA 0.5 1.0 0.4 100 50 350.0 instances/50_20_01

