//sessionsButton sql connection update
//doesnt find subject ...

#include <time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<gtk/gtk.h>
#include<gtk/gtkx.h>
#include<math.h>
#include<ctype.h>

#include <stdio.h>
#include <curses.h>
#include <Python.h>

#include </usr/include/mysql/mysql.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define SIZE 1024
#define max 80


//make them global

GtkWidget *mainWindow;
GtkWidget *mainFixed;
GtkWidget *sessionsButton;
GtkWidget *registerButton;
GtkBuilder *builder;

GtkWidget *registrationWindow;
GtkWidget *registrationFixed;
GtkWidget *idLabel;
GtkWidget *nameLabel;
GtkWidget *sectionLabel;
GtkWidget *levelLabel;
GtkWidget *errorLabel;
GtkWidget *instructionsLabel;
GtkWidget *invalidLabel;
GtkWidget *idEntry;
GtkWidget *nameEntry;
GtkWidget *sectionEntry;
GtkWidget *levelEntry;
GtkWidget *faceButton;
GtkWidget *saveButton;
GtkBuilder *builder2;

GtkWidget *newWindow;
GtkWidget *newFixed;
GtkWidget *newLabel;
GtkBuilder *builder3;

const gchar *idtmp;
const gchar *nametmp;
const gchar *sectiontmp;
const gchar *leveltmp;
gint idint;
int countFaceButton=1;

#define SA struct sockaddr


struct TIME {
   int seconds;
   int minutes;
   int hours;
};

void differenceBetweenTimePeriod(struct TIME t1,
                                 struct TIME t2,
                                 struct TIME *diff);
                                 
                                 // Computes difference between time periods
void differenceBetweenTimePeriod(struct TIME start,
                                 struct TIME stop,
                                 struct TIME *diff) {
   while (stop.seconds > start.seconds) {
      --start.minutes;
      start.seconds += 60;
   }
   diff->seconds = start.seconds - stop.seconds;
   while (stop.minutes > start.minutes) {
      --start.hours;
      start.minutes += 60;
   }
   diff->minutes = start.minutes - stop.minutes;
   diff->hours = start.hours - stop.hours;
}


int main(int argc, char *argv[]){

	gtk_init(&argc, &argv); //init Gtk
	
	//establish contact with xml code to adjust widget settings
	
	builder = gtk_builder_new_from_file("test.glade");
	
	mainWindow = GTK_WIDGET(gtk_builder_get_object(builder,"mainWindow"));
	
	g_signal_connect(mainWindow,"destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_builder_connect_signals(builder, NULL);
	
	mainFixed = GTK_WIDGET(gtk_builder_get_object(builder,"mainFixed"));
	sessionsButton = GTK_WIDGET(gtk_builder_get_object(builder,"sessionsButton"));
	registerButton = GTK_WIDGET(gtk_builder_get_object(builder,"registerButton"));
	
	gtk_widget_show(mainWindow);
	
	gtk_main();
	
	return EXIT_SUCCESS; 

}


void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp)!=NULL)
    {
        if(send(sockfd, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}



int readRaw(int sock, void *data, int size)
{
    char *buffer = (char*) data;
    while (size > 0)
    { 
        ssize_t recvd = recv(sock, buffer, size, 0);
        if (recvd < 0)
            return -1;
        if (recvd == 0)
            return 0;
        buffer += recvd;
        size -= recvd;
    }
    return 1;
}
int readInt32(int sock, int32_t *value)
{
    int ret = readRaw(sock, value, sizeof(*value));
    if (ret == 1)
        *value = ntohl(*value);
    return ret;
}

char* readString(int sock)
{
    int32_t len = 0;
    if (readInt32(sock, &len) <= 0)
        return NULL;

    char *ret = (char*) malloc(len+1);
    if (!ret)
        return NULL;

    if (readRaw(sock, ret, len) <= 0)
    {
        free(ret);
        return NULL;
    }

    ret[len] = '\0';
    return ret;
}

bool sendRaw(int sock, const void *data, int size)
{
    const char *buffer = (const char*) data;
    while (size > 0)
    { 
        ssize_t sent = send(sock, buffer, size, 0);
        if (sent < 0)
            return false;
        buffer += sent;
        size -= sent;
    }
    return true;
}

bool sendInt32(int sock, int32_t value)
{
    value = htonl(value);
    return sendRaw(sock, &value, sizeof(value));
}

bool sendString(int sock, const char *s)
{
    int32_t len = strlen(s);
    if (!sendInt32(sock, len))
        return false;
    return sendRaw(sock, s, len);
}



void write_file(int sockfd)
{
    int n; 
    FILE *fp;
    char *filename = "file2.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    if(fp==NULL)
    {
        perror("[-]Error in creating file.");
        exit(1);
    }
    while(1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if(n<=0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
    
}


int sockfd;


	
void on_sessionsButton_clicked(GtkButton *b){

	/*builder3 = gtk_builder_new_from_file("test2.glade");
	
	newWindow = GTK_WIDGET(gtk_builder_get_object(builder3,"newWindow"));
	newFixed = GTK_WIDGET(gtk_builder_get_object(builder3,"newFixed"));
	newLabel = GTK_WIDGET(gtk_builder_get_object(builder3,"newLabel"));


	g_signal_connect(newWindow,"destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_builder_connect_signals(builder3, NULL);
	
	
	
	gtk_widget_show(newWindow);
	gtk_main();*/
	gtk_widget_destroy(mainWindow);
	
	
	//find time 
	
	  struct TIME startTime, stopTime, diff;
   
   time_t my_time;
   struct tm * timeinfo; 
   time (&my_time);
   timeinfo = localtime (&my_time);
        
         startTime.hours = 9;
         startTime.minutes = 00;
         startTime.seconds = 00;

	 stopTime.hours = timeinfo->tm_hour;
         stopTime.minutes = timeinfo->tm_min;
         stopTime.seconds = timeinfo->tm_sec;

   // Difference between start and stop time

       differenceBetweenTimePeriod(startTime, stopTime, &diff);
       
       char *timeSql;
       
       switch(timeinfo->tm_hour){
       case 9:
       	timeSql = "9:00:00";
       	break;
       case 10:
       	if(timeinfo->tm_min<=30){
       	timeSql = "9:00:00";
       	
       	}else{
       	timeSql = "10:30:00";
       	
       	}
       	break;
       case 11:
       	timeSql = "10:30:00";
       	break;
       case 12:
       	timeSql = "12:00:00";
       	break;
       case 13:
       	if(timeinfo->tm_min<=30){
       	timeSql = "12:00:00";
       	}else{
       	timeSql = "13:30:00";
       	}
       	break;
       case 14:
       	timeSql = "13:30:00";
       	break;
       case 15:
       	timeSql = "15:00:00";
       	break;
       case 16:
       	if(timeinfo->tm_min<=30){
       	timeSql = "15:00:00";
       	}else{
       	timeSql = "16:30:00";
       	}
       	break;
       case 17:
       	timeSql = "16:30:00";
       	break;
       default:
       	timeSql = "00:00:00";
       
       
       }
	
	printf("%s\n", timeSql);
	
	//find day of the week
	char dayWeek1[200];
	char dayWeek[200];
       time_t t;
       struct tm *tmp;

       t = time(NULL);
       tmp = localtime(&t);
       if (tmp == NULL) {
           perror("localtime");
           exit(EXIT_FAILURE);
       }

       if (strftime(dayWeek, sizeof(dayWeek), "%A", tmp) == 0) {
           fprintf(stderr, "strftime returned 0");
           exit(EXIT_FAILURE);
       }

       printf("Today is \"%s\"\n", dayWeek);// output of dayWeek is in uzbek >< (not anymore)
	
	int connfd, len;
    struct sockaddr_in servaddr, cli;
   
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.42.220");;
    servaddr.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
        int count = 0;
        
        while(1){
   
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else{
        printf("server accept the client...\n");
        gtk_label_set_text(GTK_LABEL(newLabel),(const gchar* )"Client accepted!");
        }
  
        
   
	    // Function for chatting between client and server
	    char *buffer = readString(connfd);
	    
	    
	    printf("\nID: %s\n",buffer);
	    
	    ////////////////////////////////////////////////////////////////////////////////////////////////////
	    
	    
	    
	    
	    
	    
	    
	    ///////mySQL connection/////////
				   MYSQL *conn;
				   MYSQL_RES *res;
				   MYSQL_RES *res1;
				   MYSQL_ROW row;
				   MYSQL_ROW row1;
				   char sql_statement[2048];
				   char sql_statement1[2048];

				   char *server = "localhost";
				   char *user = "elina";
				   char *password = "Spirk@2001!"; //elina user password 
				   char *database = "project";

				   conn = mysql_init(NULL);

				   /* Connect to database */
				   if (!mysql_real_connect(conn, server,
					 user, password, database, 0, NULL, 0)) {
				      fprintf(stderr, "%s\n", mysql_error(conn));
				      exit(1);
				   }

				   /* send SQL query */
				   //find section of particular student
				   
				   sprintf(sql_statement, "SELECT section FROM student WHERE id='%s'",buffer);
				   if (mysql_query(conn, sql_statement)) {
				      fprintf(stderr, "%s\n", mysql_error(conn));
				      exit(1);
				   }

				   res = mysql_use_result(conn);
				   res1 = mysql_use_result(conn);
				   int numfields = mysql_num_fields(res);
				   char *sectionSql;
				   int sectionSqlInt;
				   
				   //create file tmp for section to transfer to int
				   FILE *subjectFile2;
				   subjectFile2 = fopen("tmp.txt", "w");

				   /* output table name */
				   printf("Section of student : ");
				  row = mysql_fetch_row(res);
				   	for(int i = 0; i < numfields; i++){
				   		sectionSql = row[i];
				   		//sscanf(sectionSql, "%d", &sectionSqlInt);//from char to int(doesnt work)
				   		if (subjectFile2 == NULL)
						    {
						    printf("Cant open");
						    }
						    fputs(sectionSql, subjectFile2);
						    fputs("\n", subjectFile2);
				   		printf("%s\n",sectionSql);
				   	}
				   	fclose(subjectFile2);   	
				   
				   mysql_free_result(res);
				   
				   //printf("hope it will appear\n");
				   
				   //file for section to transfer to int 
				   
				   char forFile;
				   FILE *subjectFile1;
				   subjectFile1 = fopen("tmp.txt", "r");
				   forFile = fgetc(subjectFile1);
				   int num = atoi(&forFile); 
				   printf("%d\n",num);
				   fclose(subjectFile1);
				   
				   //find subject, start_time, end_time in mysql of student
				   
				   /* send SQL query */
				   
				   sprintf(sql_statement1, "SELECT subject FROM schedule WHERE section=%d AND day='%s' AND start_time = '%s'",num,dayWeek,timeSql);
				   //sprintf(sql_statement1, "SELECT subject FROM schedule WHERE section=1 AND day='Monday' AND start_time = '10:30'");
				   if (mysql_query(conn, sql_statement1)) {
				      fprintf(stderr, "%s\n", mysql_error(conn));
				      exit(1);
				   }

				   res1 = mysql_use_result(conn);
				   numfields = mysql_num_fields(res1);
 
				   /* output table name */
				   
				   
				   row1 = mysql_fetch_row(res1);
				   if(row1 == NULL){
				   sendString(connfd, "No classes available.");
				   }else{
				   	printf("Subject : ");
				   	for(int i = 0; i < numfields; i++){
				   		sectionSql = row1[i];
				   		printf("%s\n",sectionSql);
				   	}
				   printf("\n");
				   sendString(connfd, sectionSql);
				   }

				   /* close connection */
				   
				   mysql_free_result(res1);
				   mysql_close(conn);

	    ////////////////////////////////////////////////////////////////////////////////////////////////////

	   //Read Picture Size
printf("Reading Picture Size\n");
int size;
read(connfd, &size, sizeof(int));

//Read Picture Byte Array and Copy in file
printf("Reading Picture Byte Array\n");
char p_array[size];
char *filenewname = buffer;
FILE *image = fopen("client.jpeg", "w");
int nb = read(connfd, p_array, size);
while (nb > 0) {
    fwrite(p_array, 1, nb, image);
    nb = read(connfd, p_array, size);
}

fclose(image);




char filename[] = "facerec.py";
		FILE* fp;
		int argc;
	    	char *argv[2];
	    	argc = 2;
	    	argv[0] = "takePhoto.py";
	    	argv[1] = buffer;

	    	wchar_t *program[2];
	    	program[0] = Py_DecodeLocale(argv[0], NULL);
	    	program[1]= Py_DecodeLocale(argv[1], NULL);
		
				Py_SetProgramName(program[0]);
				/*if(countFaceButton==1){ //when Py_Initialize or Py_Finalize called more than once segmantation fault will occure 
				Py_Initialize();
				}*/
				Py_Initialize();
				PySys_SetArgv(argc, program);
				fp = _Py_fopen(filename, "r");
				PyRun_SimpleFile(fp, filename);
				//Py_Finalize();
				
		for(int i=0;i<argc;i++){
			program[i] = NULL;
		}
	    
	    
	    close(connfd);
	    
	    
	    
	    
	    
	    
    }
   
    // After chatting close the socket
    close(sockfd);
}



void on_registerButton_clicked(GtkButton *b){

	
	
	builder2 = gtk_builder_new_from_file("test1.glade");
	
	registrationWindow = GTK_WIDGET(gtk_builder_get_object(builder2,"registrationWindow"));
	registrationFixed = GTK_WIDGET(gtk_builder_get_object(builder2,"registrationFixed"));
	idLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"idLabel"));
	nameLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"nameLabel"));
	sectionLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"sectionLabel"));
	levelLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"levelLabel"));
	errorLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"errorLabel"));
	instructionsLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"instructionsLabel"));
	invalidLabel = GTK_WIDGET(gtk_builder_get_object(builder2,"invalidLabel"));
	idEntry = GTK_WIDGET(gtk_builder_get_object(builder2,"idEntry"));
	nameEntry = GTK_WIDGET(gtk_builder_get_object(builder2,"nameEntry"));
	sectionEntry = GTK_WIDGET(gtk_builder_get_object(builder2,"sectionEntry"));
	levelEntry = GTK_WIDGET(gtk_builder_get_object(builder2,"levelEntry"));
	faceButton = GTK_WIDGET(gtk_builder_get_object(builder2,"faceButton"));
	saveButton = GTK_WIDGET(gtk_builder_get_object(builder2,"saveButton"));
	
	
	
	g_signal_connect(registrationWindow,"destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_builder_connect_signals(builder2, NULL);
	
	
	
	gtk_widget_show(registrationWindow);
	gtk_main();
	
}

void on_faceButton_clicked(GtkButton *b){
	
	//////take photo for database////////
	
	if(idtmp==NULL || nametmp==NULL || sectiontmp==NULL || leveltmp==NULL ){
		printf("Not full fields \n"); //comment for command line
	}else{
	
		char filename[] = "takePhoto.py";
		FILE* fp;
		int argc;
	    	char *argv[2];
	    	argc = 2;
	    	argv[0] = "takePhoto.py";
	    	argv[1] = idtmp;

	    	wchar_t *program[2];
	    	program[0] = Py_DecodeLocale(argv[0], NULL);
	    	program[1]= Py_DecodeLocale(argv[1], NULL);
		
				Py_SetProgramName(program[0]);
				if(countFaceButton==1){ //when Py_Initialize or Py_Finalize called more than once segmantation fault will occure 
				Py_Initialize();
				}
				//Py_Initialize();
				PySys_SetArgv(argc, program);
				fp = _Py_fopen(filename, "r");
				PyRun_SimpleFile(fp, filename);
				//Py_Finalize();
				
		for(int i=0;i<argc;i++){
			program[i] = NULL;
		}
	}	
	//////////////////////////////////////
}





void on_saveButton_clicked(GtkButton *b){
	
	/////////////////////mySql////////////////
	   MYSQL *conn;
	   //MYSQL_RES *res;
	   //MYSQL_ROW row;
	   char sql_statement[2048];
	   
	   gint sectionint;
	   if (sectiontmp == NULL || idtmp ==NULL || nametmp ==NULL || leveltmp ==NULL) { 
        		gtk_label_set_text(GTK_LABEL(errorLabel),(const gchar* )"Fill all fields!");
	   }else{
	   sectionint = atoi(sectiontmp);
	   idint = atoi(idtmp);
	   
	   
	   if(sectionint>=1 && sectionint<=3){
	   	   if(*leveltmp == 'J' || *leveltmp == 'F' || *leveltmp == 'S'|| *leveltmp == 'E'){
		   char *server = "localhost";
		   char *user = "elina";
		   char *password = "Spirk@2001!"; 
		   char *database = "project";
		   
		   
		   conn = mysql_init(NULL);

		   /* Connect to database */
		   if (!mysql_real_connect(conn, server,
			 user, password, database, 0, NULL, 0)) {
		      fprintf(stderr, "%s\n", mysql_error(conn));
		      exit(1);
		   }
		   sprintf(sql_statement, "INSERT INTO student VALUES('%s','%s',%d,'%s')", idtmp,nametmp,sectionint,leveltmp);
		  if(mysql_query(conn, sql_statement) !=0)
		  {
			    printf("Query failed  with the following message:\n");
			    printf("%s\n", mysql_error(conn));
			    exit(1);
		  }
		  mysql_close(conn);
		  
		  /////////////////////////////////////////////////
		  
		  gtk_widget_destroy(registrationWindow);
		  }
		  }else{
		  	gtk_label_set_text(GTK_LABEL(invalidLabel),(const gchar* )"Invalid section or level");
		  }

	   }	
	   
	   
}



void on_idEntry_changed(GtkEntry *e){
	idtmp = gtk_entry_get_text(e);
	
}

void on_nameEntry_changed(GtkEntry *e){
	nametmp = gtk_entry_get_text(e);
}
void on_sectionEntry_changed(GtkEntry *e){
	sectiontmp = gtk_entry_get_text(e);
}
void on_levelEntry_changed(GtkEntry *e){
	leveltmp = gtk_entry_get_text(e);
}




