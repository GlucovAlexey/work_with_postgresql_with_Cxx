#include <bits/stdc++.h>
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;

int main(){
	string sql;
	try {
		connection C("dbname = mydb user = alex password = 2501\
			hostaddr = 127.0.0.1 port = 5432");
		if (C.is_open()){
			cout<<"Opened database successfully: "<< C.dbname()<<endl;
		}
		else {
			cout<<"Can't open database"<<endl;
			return 1;
		}

		sql = "INSERT INTO accounts (userid,firstname,lastname,emailaddress,telephonenumber) "  \
         "VALUES (1, 'Paul', 'Worker', 'paul123@yandex.ru', '+7 495 123 18 29' );";	

         work W(C);

         W.exec(sql);
         W.commit();
         cout << "Records created successfully" << endl;
         C.disconnect ();
	}
	catch (const std::exception &e){
		cerr << e.what() << std::endl;
      	return 1;
	}
	return 0;
}