#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<gtk/gtk.h>
#include<gtk/gtkx.h>
#include<math.h>
#include<ctype.h>
#include <stdbool.h>

#include <stdio.h>
#include <curses.h>
#include <Python.h>

#include </usr/include/mysql/mysql.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
#define SIZE 1024
#define SA struct sockaddr
#define max 80
#define LSIZ 128 
#define RSIZ 10 

GtkWidget *logWindow;
GtkWidget *logFixed;
GtkWidget *idLabel;
GtkWidget *idEntry;
GtkWidget *headerLabel;
GtkWidget *logButton;
GtkWidget *classWindow;
GtkWidget *classFixed;
GtkWidget *currentClassLabel;
GtkWidget *classLabel;
GtkWidget *attendButton;
GtkBuilder *builder;
GtkBuilder *builder1;

//for entry
const gchar *idtmp;
const gchar *nametmp;


int main(int argc, char *argv[]){

	gtk_init(&argc, &argv); //init Gtk
	
	//establish contact with xml code to adjust widget settings
	
	builder = gtk_builder_new_from_file("client.glade");
	
	logWindow = GTK_WIDGET(gtk_builder_get_object(builder,"logWindow"));
	
	g_signal_connect(logWindow,"destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_builder_connect_signals(builder, NULL);
	
	logFixed = GTK_WIDGET(gtk_builder_get_object(builder,"logFixed"));
	idLabel = GTK_WIDGET(gtk_builder_get_object(builder,"idLabel"));
	idEntry = GTK_WIDGET(gtk_builder_get_object(builder,"idEntry"));
	headerLabel = GTK_WIDGET(gtk_builder_get_object(builder1,"headerLabel"));
	logButton = GTK_WIDGET(gtk_builder_get_object(builder,"logButton"));
	
	gtk_widget_show(logWindow);
	
	gtk_main();
	
	
	return EXIT_SUCCESS; 

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

void func(int sockfd)
{
    char buff[max];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}
int countFaceButton=1;
void on_logButton_clicked(GtkButton *b){

	char buffer[32];
	
	builder1 = gtk_builder_new_from_file("client1.glade");
	
	classWindow = GTK_WIDGET(gtk_builder_get_object(builder1,"classWindow"));
	classFixed = GTK_WIDGET(gtk_builder_get_object(builder1,"classFixed"));
	currentClassLabel = GTK_WIDGET(gtk_builder_get_object(builder1,"currentClassLabel"));
	classLabel = GTK_WIDGET(gtk_builder_get_object(builder1,"classLabel"));
	attendButton = GTK_WIDGET(gtk_builder_get_object(builder1,"attendButton"));
	
	g_signal_connect(classWindow,"destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_builder_connect_signals(builder1, NULL);
	
	
	
	//FACE RECOGNITION

	//////take photo for database////////

	
		char filename[] = "takePhoto.py";
		FILE* fp;
		int argc;
	    	char *argv[2];
	    	argc = 2;
	    	argv[0] = "takePhoto.py";
	    	argv[1] = "client1";

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
				Py_Finalize();
				
		for(int i=0;i<argc;i++){
			program[i] = NULL;
		}
		
		
		
		
	//////////////////////////////////////
	

	///////////////////CLIENT SIDE//////////////////////////

 int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
   
    // socket create and varification
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
    servaddr.sin_addr.s_addr = inet_addr("192.168.42.220");
    servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
		
   
    // function for chat
    sprintf(buffer, "%s", idtmp);
    sendString(sockfd, buffer);
    
    char *newbuffer = readString(sockfd);
    printf("%s\n",newbuffer);

    gtk_label_set_text(GTK_LABEL(classLabel),(const gchar* )newbuffer);
    
    //Get Picture Size
printf("Getting Picture Size\n");
FILE *picture;
picture = fopen("client1.jpeg", "r");
int size;
fseek(picture, 0, SEEK_END);
size = ftell(picture);
fseek(picture, 0, SEEK_SET);

//Send Picture Size
printf("Sending Picture Size\n");
write(sockfd, &size, sizeof(size));

//Send Picture as Byte Array (without need of a buffer as large as the image file)
printf("Sending Picture as Byte Array\n");
char send_buffer[size]; // no link between BUFSIZE and the file size
int nb = fread(send_buffer, 1, sizeof(send_buffer), picture);
while(!feof(picture)) {
    write(sockfd, send_buffer, nb);
    nb = fread(send_buffer, 1, sizeof(send_buffer), picture);
    // no need to bzero
}
   
    // close the socket
    close(sockfd);

	//////////////////////////////////////////////////////////////
	
	gtk_widget_destroy(logWindow);
	gtk_widget_show(classWindow);
	gtk_main();
		
}

void on_idEntry_changed(GtkEntry *e){
	idtmp = gtk_entry_get_text(e);	
}

void on_nameEntry_changed(GtkEntry *e){
	nametmp = gtk_entry_get_text(e);	
}


void on_attendButton_clicked(GtkButton *b)
{
	gtk_widget_destroy(classWindow);
	//gtk_widget_show(logWindow);

}
















