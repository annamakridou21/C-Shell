 /*ANNA MAKRIDOU 4934*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void type_prompt(char*username,char*arr,char*buff){
if(username!=NULL){
	strcpy(arr,"4934-hy345sh@"); /*TO AM MOU + HY345SH + @*/
	strcat(arr,username);
	strcat(arr,": ");	
	strcat(arr,buff); /*trexon katalogos*/
}
printf("%s ",arr);
}

char check_symbol(char* buffer){ /*sunarthsh pou elegxei ti eidous entolh zhtaei o xrhsths*/
int i,where=0;
if(strchr(buffer,';')!=NULL){/*an uparxei estw kai ena erwthmatiko shmainei pws exoume multiple commands*/
	return 'm';/*m for multiple commands*/	
}
else if(strchr(buffer,'|')!=NULL){/*an uparxei estw kai ena pipe operator shmainei pws exoume pipe*/
	return 'p';/*p for pipe*/	
}
else if (strchr(buffer,'>')!=NULL){/*an uparxei estw kai ena > den eimaste vevaioi pws einai gia output redirection*/
	i=0;
	while(buffer[i]!='\0'){
	   if(buffer[i]=='>'){/*entopizoume thn prwth emfanish tou sumvolou > sto input*/
		where=i;
		break;
	   } 
	   i++;
	}
	if(buffer[where+1]!='\0'){/*an uparxei kati meta to > elegxoume an uparxei deutero >*/
	   if(buffer[where+1]=='>'){ /*an uparxei > meta thn prwth emfanish tou > tote exoume append*/
	   	return 'a';	/*a for append*/
	   }
	   else return 'o'; /*o for redirected output*/
	}
}
else if(strchr(buffer,'<')!=NULL){ /*an uparxei < tote exoume redirected input*/
	return'i';/*i for redirected input*/
}
else return 's';/*an den isxuei tipota apo ta parapanw tote ekteloume aplh entolh*/
return '\0'; /*s for simple command*/
}

int main(void){
char buffer[1000],s[1000],buff[1000],home[1000],arr[1000],buff_copy[1000],*username,*word[1000],*mul_com[100],*a,ret;
int i,len,x,j,count,where=0,fp,pipefd[2],success,pid;
getcwd(buff,1000); /*pairnw to path gia to directory pou  vriskomai*/
strcpy(home,buff);/*krataw to arxiko path se periptwsh pou allaksei argotera logw chdir*/
username=getenv("USER");/*pairnnw to onoma tou user*/
while (1) {
	type_prompt(username,arr,buff);
	fgets(buffer, 1000, stdin); /*h sunarthsh fgets prosthetei newline sto input*/
	if(strcmp(buffer,"\n")==0)continue; /*an dothei enter ksanazhtaw input*/
	len=strlen(buffer);
	if (buffer[len-1]=='\n') {	
		buffer[len-1]='\0';/*an exei prostethei newline to antikathistw me null terminator gia na fugei apto input*/
 	}
	if(strcmp(buffer,"quit")==0) exit(0);/*an mou dwsei o xrhsths quit vgainw apto shell-termatismos programmatos*/
	i=0;
 	while(buffer[i]!='\0'){
 	   buff_copy[i]=buffer[i];/*ftiaxnw ena antigrafo tou command gia na elenksw an einai cd*/
 	   i++;
 	}
 	buff_copy[i]='\0';
 	a=strtok(buff_copy," ");/*krataw mono to command gia na eleksw an einai cd*/
 	ret=check_symbol(buffer);/*elegxw to input tou xrhsth*/
 	if(ret=='s' && strcmp(a,"cd")==0){
 	   a=strtok(NULL," ");/*diaxwrizw tis parametrous tou cd*/
 	   if(a==NULL){
 		getcwd(buff,64);/*an den exw parameters tote menw sto idio directory*/
 		chdir(buff);
 	   }
 	   else if((strcmp(a,"..")==0) || strcmp(a,"-")==0){	
 		i=0;  /*.. kai - shmainoun metakinhsh se prohgoumeno directory*/
 		count=0;
 		while(buff[i]!='\0'){/*psaxnw pou teleiwnei to prohgoumeno directory*/
 		   if(buff[i]=='/'){
 			where=i;
 			count++;
 		   }
 		   i++;
 		}
 		if(count>1)buff[where]='\0';/*kai se ekeino to shmeio stamataw to buffer*/
 		chdir(buff);/*afou allaksa to message sto prohgoumeno dir,metakinoume se auto.*/
 	   }
 	   else {	
 	   	strcpy(s,buff);/*se kathe allh periptwsh px cd dir ftiaxnw ena neo path*/
 		strcat(s,"/");/*prosthetw to dir sto hdh uparxon path*/
 		strcat(s,a);/*metakinoumai ston dir*/
 		chdir(s);/*allazw to message tou shell gia na fainetai h metakinhsh*/
 		strcpy(buff,s);
 	   }
 	}
 	if(ret=='s'){/*an exei dwsei aplh entolh*/
 	   a=strtok(buffer," ");
 	   pid=fork();/*an den einai h entolh cd tote kanoume fork*/
 	   if (pid==0){/*an einai epituxhmeno to fork*/
 	   	word[0]=a;/*to prwto parameter ths exec einai to command to opoio prepei na topothetithei sth prwth thesh pinaka*/
 		i=1;
 		a=strtok(NULL, " ");/*spame to string se kommatia kathe fora pou sunantaw space*/
		while (a!=NULL) {
 			word[i]=a;	/*vazoume ta arguments tou command stis upoloipes theseis tou pinaka*/
 			a=strtok(NULL, " ");
			 i++;
 		}	
 		word[i]=NULL; 	/*o pinakas prepei na teleiwnei se NULL*/
 		success=execvp(word[0], word);
 		if(success==-1 && strcmp(word[0],"cd")!=0) printf("Not a command!\n");
 		exit(0);
 	   }
 	   else if(pid>0) waitpid(-1,NULL,0);	/*perimenw to paidi na kanei exit*/
 	   else printf("Fork not executed succesfuly!\n");
 	}
 	else if(ret=='i'){	/*redirected input*/
  	   a=strtok(buffer," ");
  	   pid=fork();
  	   if (pid==0){
  	   	word[0]=a;
  		i=1; 
  		a=strtok(NULL, " ");
		while (*a!='<') {
		   word[i]=a; /*mexri na sunanthsw to < krataw se enan pinaka to command sth thesh 0 kai ta args tou*/
		   a=strtok(NULL, " ");
		   i++;
		}
		word[i]=NULL;
		a=strtok(NULL, "<");/*vriskw to string meta to <*/
		fp=open(a, O_RDONLY);/*to opoio einai txt file kai to anoigw*/
 		if(fp==-1)exit(1);
		dup2(fp,STDIN_FILENO);/*redirect to file pou edwse o xrhsths sto stdin*/
		close(fp);/*kleinw to file*/
		success=execvp(word[0], word);/*ektelw to kommati aristera tou < pou to eixa apothikeusei sto array*/
		if(success==-1) printf("Execvp not executed succesfuly!\n");
		exit(0);
	}
	else if(pid>0) waitpid(-1,NULL,0);
	else printf("Fork not executed succesfuly!\n");
	}
	else if(ret=='o'){/*redirected output*/
	   a=strtok(buffer," ");
	   pid=fork();
	   if (pid==0){
		word[0]=a;
		i=1;
		a=strtok(NULL, " ");
		while (*a!='>') {/*krataw se ena pinaka to command kai ta args tou*/
		   word[i]=a;
		   a=strtok(NULL, " ");
		   i++;
		}
		word[i]=NULL;
		a=strtok(NULL, ">");/*krataw sto a to kommati meta to >*/
		fp=open(a, O_WRONLY | O_TRUNC | O_CREAT);/*to arxeio auto anoigei gia na grapsw. an den uparxei tha dhmiourghthei kai an exw hdh grapsei se auto me to O_TRUNC tha diagrafei to periexomeno tou gia to epomeno output*/
		if(fp==-1)fp=creat(a,O_RDWR);/*se periptwsh pou apotuxei h parapanw entolh dhmiourgw to file ksana*/
		dup2(fp,STDOUT_FILENO); /*redirect to file tou xrhsth sto stdout*/	
		close(fp);
		success=execvp(word[0], word);
		if(success==-1) printf("Execvp not executed succesfuly!\n");
		exit(0);
	}
	else if(pid>0) waitpid(-1,NULL,0);
	else printf("Fork not executed succesfuly!\n");
	}
	else if(ret=='a'){	
	        a=strtok(buffer," ");
	 	pid=fork();
		if (pid==0){
			word[0]=a;
			i=1;
			a=strtok(NULL, " ");
			while (*a!='>') {
				word[i]=a;
				a=strtok(NULL, " ");
				i++;
			}
			word[i]=NULL;  
			a=strtok(NULL, ">");				
			fp=open(a, O_WRONLY | O_CREAT | O_APPEND); /*edw den theloume na diagraftei to prohgoumemo periexomeno giauto vazoume to append gia na mpei to neo text sto telos tou file*/
			if(fp==-1)exit(1);
			dup2(fp,STDOUT_FILENO);
			close(fp);
			success=execvp(word[0], word);
			if(success==-1) printf("Execvp not executed succesfuly!\n");
				exit(0);
			}
		else if(pid>0) waitpid(-1,NULL,0);
		else printf("Fork not executed succesfuly!\n");
	}	
	else if(ret=='p'){
	   x=pipe(pipefd);
 	   if(x==-1)printf("Pipe not executed succesfuly!\n");
	   a=strtok(buffer,"|"); /*vazw se ena pinaka ola ta kommatia tou pipe*/
 	   i=0;
 	   while(a!=NULL){
 		mul_com[i]=a;
 		a=strtok(NULL,"|");
 		i++;
 	   }
	   for(x=0;x<i;x++){/*gia kathe meros tou pipe theloume allh diergasia. exoume i entoles sto pipe ara i diergasies*/
 	   	a=strtok(mul_com[x], " ");	
 		pid=fork();	
 		if (pid==0){
 		   word[0]=a;
 		   j=1;
 		   a=strtok(NULL, " ");/*diaxwrizw to prwto meros tou pipe se commands kai args*/
		   while (a!=NULL) {
 			word[j]=a;
 			a=strtok(NULL, " ");
 			j++;
 		   }
 		   word[j]=NULL; /*DOULEUEI MONO GIA ENA PIPE OPERATOR | */
		   if(x==0){ /*prwth entolh*/
			close(pipefd[0]);/*h prwth entolh thelei na grapsei sto stdout ara kleinoume to read end*/
			dup2(pipefd[1], STDOUT_FILENO); /*to pipefd[1] antistoixei sto write end giauto kanw redirect to write end sto to stdout*/
			close(pipefd[1]);/*afou grapsw sto write end to kleinw*/
		   }
 		   if(x==1){/*deuterh entolh*/
  			close(pipefd[1]); /*twra thelw na diavasw to prohgoumeno kai na to emfanisw sto stdin*/
  			dup2(pipefd[0], STDIN_FILENO);/*redirect to read end sto stdin*/
  			close(pipefd[0]);/*kleinw to read end*/
  		   }
  		   execvp(word[0],word);/*ektelw thn entolh*/
   		   exit(0);
   		}
   	   }
   	   close(pipefd[0]);/*elegxw ksana an ekleise*/
   	   close(pipefd[1]);
	}
	else if(ret=='m'){
	   a=strtok(buffer,";");
	   i=0;
 	   while(a!=NULL){
 		mul_com[i]=a;/*antistoixa me to pipe krataw se ena pinaka oles tis entoles pou diaxwrizontai me ;*/
 		a=strtok(NULL,";");
		 i++;
 	   }
 	   for(x=0;x<i;x++){
 	      ret=check_symbol(mul_com[x]);
 	      a=strtok(mul_com[x], " ");	/*epeksergazomai mia mia tis entoles*/
 	      if(strcmp(a,"cd")==0){/*elegxw prwta ama kapoia apo autes einai h cd*/
	         a=strtok(NULL," ");
 		 if(a==NULL){
 		    strcpy(buff,home);
 		    chdir(buff);
 		 }
 	         else if((strcmp(a,"..")==0) || strcmp(a,"-")==0){
 	            i=0;
 		    count=0;
 		    while(buff[i]!='\0'){
 		       if(buff[i]=='/'){
 		   	   where=i;
 			   count++;
 		       }
 		       i++;
 		    }
 		    if(count>1)buff[where]='\0';
 		    chdir(buff);
 	            }
 	            else {
 			strcpy(s,buff);
 			strcat(s,"/");
 			strcat(s,a);
 			chdir(s);
 			strcpy(buff,s);	
 	      	    }
 	      }
 	      else {/*gia kathe mia apo tis polles entoles elegxw ti eidous entolh einai*/
 	         pid=fork();/*an den einai h entolh cd tote kanoume fork*/
 		 if (pid==0){ 
 		 	if(ret=='s'){/*an exei dwsei aplh entolh*/
 			   word[0]=a;/*to prwto parameter ths exec einai to command to opoio prepei na topothetithei sth prwth thesh pinaka*/
 			   i=1;
 			   a=strtok(NULL, " "); /*spame to string se kommatia kathe fora pou sunantaw space*/
 			   while (a!=NULL) {
 			   	word[i]=a; /*vazoume ta arguments tou command stis upoloipes theseis tou pinaka*/
 				a=strtok(NULL, " ");
 				i++;
 			   }
 			   word[i]=NULL;/*o pinakas prepei na teleiwnei se NULL*/
 			   success=execvp(word[0], word);
 			   if(success==-1) printf("Execvp not executed succesfuly!\n");
 			   exit(0);
 			}
 			else if(ret=='i'){ /*redirected input*/
 			   word[0]=a;/*to vazw sth prwth thesh tou array*/
 			   i=1;
 			   a=strtok(NULL, " ");
 			   while (*a!='<') { /*mexri na sunanthsw to < krataw se enan pinaka to command kai ta args tou*/
 				word[i]=a;
 				a=strtok(NULL, " ");
 				i++;
 			   }
 			   word[i]=NULL;
 			   a=strtok(NULL, "<"); /*vriskw to string meta to <*/
 			   fp=open(a, O_RDONLY);/*to opoio einai txt file kai to anoigw*/
 			   if(fp==-1)exit(1);
			   dup2(fp,STDIN_FILENO);
			   close(fp);
			   success=execvp(word[0], word);
			   if(success==-1) printf("Execvp not executed succesfuly!\n");
			   exit(0);
		        }
			else if(ret=='o'){ 
			   word[0]=a;
			   i=1;
			   a=strtok(NULL, " ");
		           while (*a!='>') {
			   	word[i]=a;
				a=strtok(NULL, " ");
				i++;
			   }		/*epanalhpsh kwdika*/
			   word[i]=NULL;
			   a=strtok(NULL, ">");
			   fp=open(a, O_WRONLY | O_TRUNC | O_CREAT);
			   if(fp==-1)fp=creat(a,O_RDWR);
			   dup2(fp,STDOUT_FILENO); 
			   close(fp);
			   success=execvp(word[0], word);
			   if(success==-1) printf("Execvp not executed succesfuly!\n");
			   exit(0);
			}
			else if(ret=='a'){  
			   word[0]=a;
			   i=1;
			   a=strtok(NULL, " ");
			   while (*a!='>') {
				word[i]=a;
				a=strtok(NULL, " ");
				i++;
			   }
			   word[i]=NULL;  
			   a=strtok(NULL, ">"); 
			   fp=open(a, O_WRONLY | O_CREAT | O_APPEND);
   			   if(fp==-1)exit(1);
			   dup2(fp,STDOUT_FILENO);
			   close(fp);
		           success=execvp(word[0], word);
			   if(success==-1) printf("Execvp not executed succesfuly!\n");
			   exit(0);
			}
		}
		else if(pid>0) waitpid(-1,NULL,0);
		else printf("Fork not executed succesfuly!\n");
		}
	   }
	}
   }
return 0;
}
