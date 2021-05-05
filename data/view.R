install.packages("RSQLite")
library("RSQLite")

setwd("C:/R/Course work")

sqlite    <- dbDriver("SQLite")
db <- dbConnect(sqlite,"packing_trace_zone_a_v1.sqlite")

dbListTables(db)

vm <- dbReadTable(db, 'vm')
vmType <- dbReadTable(db, 'vmType')

write.csv(x=vm, file="vm.csv")
write.csv(x=vmType, file="vmType.csv")
