import re
from turtle import up 

def sortQueries(file_name):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    new_file = open("data/queries/" +
                file_name + "_cardiality_asc_queries", "w")

    next(file)
    data = []
    for line in file:
        line = line.split("\t")

        for i in range(0,len(line)):
            line[i] = float(line[i].replace("\n", ""))
        
        data.append(line)


    data.sort(key=lambda row: (row[7]), reverse=True)

    for line in data:
        new_file.write(
            str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(line[2]) + "\t" + str(line[3]) + "\t" + str(line[4])  + "\t" + str(line[5])  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
        )

def findQueriesWithDegreAndArea(file_name, degree=False, area=False, cardinality= False, maxDegree=600, amount_of_queries=1000):
    file = open("data/queries/" +
                file_name + "_queries", "r")

    degreeStr = str(degree) if degree else "all"
    areaStr = str(area) if area else "all"
    cardinalityStr = str(cardinality) if cardinality else "all"

    new_file = open("data/queries/" +
                file_name + "_degree_" + degreeStr + "_area_" + areaStr + "_cardinality_" + cardinalityStr, "w")

    next(file)
    data = []
    for line in file:
        line = line.split("\t")

        for i in range(0,len(line)):
            line[i] = float(line[i].replace("\n", ""))
        
        if int(line[1]) > 700: 
            continue

        if degree:
            if not (degree - (degree*0.5) <= int(line[1]) <= degree + (degree*0.5)):
                continue
        if area:
            if float(line[6]) != area:
                continue
        if cardinality:
            if  not (cardinality - (cardinality * 0.1) <= int(line[7]) <= cardinality + (cardinality * 0.1)):
                continue

        data.append(line)

    # if degree:
    #     data.sort(key=lambda row: (row[1]), reverse=True)
    # if area: 
    #     data.sort(key=lambda row: (row[6]), reverse=True)
    # if cardinality: 
    #     data.sort(key=lambda row: (row[7]), reverse=True)

    if (len(data) > amount_of_queries): 
        take_every_n_entry = int(len(data)/ amount_of_queries)

        print("amount of queries: ", len(data) / take_every_n_entry, )
        counter = 0
        for line in data:
            if counter % take_every_n_entry == 0:
                new_file.write(
                    str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(f'{line[2]:f}') + "\t" + str(f'{line[3]:f}') + "\t" + str(f'{line[4]:f}')  + "\t" + str(f'{line[5]:f}')  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
                )
            counter = counter + 1 

    else: 
        print("amount of queries: ", len(data))
        for line in data:
            new_file.write(
                str(int(line[0])) + "\t" +  str(int(line[1])) + "\t" + str(f'{line[2]:f}') + "\t" + str(f'{line[3]:f}') + "\t" + str(f'{line[4]:f}')  + "\t" + str(f'{line[5]:f}')  + "\t" + str(line[6]) + "\t" + str(int(line[7])) + "\n"
            )

def getAllTrueQueries(seperator, result_file, write_true_cases = False):
    # query_true_file = open("data")
    result_file = open("data/results/"  + result_file)
    next(result_file)
    counter_true = 0
    counter_false = 0
    if write_true_cases: 
        new_query_file = open("data/queries/yelp_wcc_queries_true", "w")
        new_query_file.write("node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality\tcity\n")
        new_query_file_area_true = open("data/queries/yelp_wcc_area_queries_true", "w")
        new_query_file_area_true.write("node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality\tcity\n")
        new_query_file_degree_true = open("data/queries/yelp_wcc_degree_queries_true", "w")
        new_query_file_degree_true.write("node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality\tcity\n")

        new_query_file_area_false = open("data/queries/yelp_wcc_area_queries_false", "w")
        new_query_file_area_false.write("node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality\tcity\n")
        new_query_file_degree_false = open("data/queries/yelp_wcc_degree_queries_false", "w")
        new_query_file_degree_false.write("node\tdegree\txLow\tyLow\txTop\tyLow\tarea\tcardinality\tcity\n")

        area_result_dict = {}
        degree_result_dict = {
            100 : 0,
            50 : 0,
            10: 0,
            5: 0,
            1: 0
        }
        area_result_dict_true = {}
        degree_result_dict_true = {
            100 : 0,
            50 : 0,
            10: 0,
            5: 0,
            1: 0
        }

        for line in result_file:
            line = line.replace("\n", "")
            line = line.split(seperator)

            if line[1] == "1":
                new_query_file.write(line[0] + "\t" + line[3] + "\t" + line[5] + "\t" + line[6] + "\t" + line[7] + "\t" + line[8] + "\t" + line[2] + "\t" + line[4] + "\t" + line[9] + "\n")
                counter_true = counter_true + 1

                if int(line[3]) > 10 and int(line[3]) < 51:

                    if line[2] in area_result_dict_true:
                        area_result_dict_true[line[2]] += 1
                    else: 
                        area_result_dict_true[line[2]] = 1

                    if(area_result_dict_true[line[2]] < 300):
                        new_query_file_area_true.write(line[0] + "\t" + line[3] + "\t" + line[5] + "\t" + line[6] + "\t" + line[7] + "\t" + line[8] + "\t" + line[2] + "\t" + line[4] + "\t"+ line[9] + "\n")

                if line[2] ==  "0.01":
                    for upper_bucket_limit in degree_result_dict:
                        if int(line[3]) > upper_bucket_limit:
                            degree_result_dict_true[upper_bucket_limit] += 1
                            if degree_result_dict_true[upper_bucket_limit] < 400: 
                                new_query_file_degree_true.write(line[0] + "\t" + line[3] + "\t" + line[5] + "\t" + line[6] + "\t" + line[7] + "\t" + line[8] + "\t" + line[2] + "\t" + line[4] + "\t" + line[9] + "\n")
                            break
            else: 
                counter_false = counter_false + 1
            

                if int(line[3]) > 10 and int(line[3]) < 50:
                    if line[2] in area_result_dict:
                        area_result_dict[line[2]] += 1
                    else: 
                        area_result_dict[line[2]] = 1
                    
                    if area_result_dict[line[2]] < 300: 
                        new_query_file_area_false.write(line[0] + "\t" + line[3] + "\t" + line[5] + "\t" + line[6] + "\t" + line[7] + "\t" + line[8] + "\t" + line[2] + "\t" + line[4] + "\t" + line[9] + "\n")

                if line[2] ==  "0.01":
                    for upper_bucket_limit in degree_result_dict:
                        if int(line[3]) > upper_bucket_limit:
                            degree_result_dict[upper_bucket_limit] += 1
                            if degree_result_dict[upper_bucket_limit] < 300: 
                                new_query_file_degree_false.write(line[0] + "\t" + line[3] + "\t" + line[5] + "\t" + line[6] + "\t" + line[7] + "\t" + line[8] + "\t" + line[2] + "\t" + line[4] + "\t" + line[9] + "\n")
                            break
    else:

        for line in result_file:
            line = line.split(seperator)

            if line[1] == "1":
                counter_true = counter_true + 1
            else: 
                counter_false = counter_false + 1
    print(counter_true, " " , counter_false)

    print("true")
    print(area_result_dict_true)
    print(degree_result_dict_true)

    print("false")
    print(degree_result_dict)
    print(area_result_dict)


# print("Enter filename:")
# file = input()

file = "yelp_wcc"

print("All:")
getAllTrueQueries("\t", "test/" + file + "_bfs", True)


# print("Area:")
# findQueriesWithDegreAndArea(file, area=0.1, amount_of_queries=1500)
# getAllTrueQueries(" ", "test/" + file + "_bfs_area")

# print("Degree:")
# findQueriesWithDegreAndArea(file, degree=25, amount_of_queries=1500)
# getAllTrueQueries(" ", "test/" + file + "_bfs_degree")




# sortQueries("foursquare")