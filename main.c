#include<stdio.h>
#include<string.h>
#define strsize 80
#define arrsize 100
#define total_no_courses 3
#define marks_criteria 1//only jee for now
//course_rating[i]=-1 means course not available
//course rating is out of 10
//arr[0]=cse,1=mech,2=ee
#define total_colleges 100
char total_course_names[][strsize]={"COMPUTER SCIENCE ENGG.","MECHANICAL ENGG.","ELECTRICAL ENGG."};
struct student
{
	char name[strsize];
	unsigned int marks[marks_criteria];
	unsigned int preference[total_no_courses];
};

struct course
{
	int rating;
	unsigned int no_seats;
	char name[strsize];
	int min_marks[marks_criteria];
	int no;
};
struct college
{
	char name[strsize];
	struct course courses[total_no_courses];
	int no_courses;

};
void course_input(struct course *t, int j)
{
	fflush(stdin);
	int i,ch;
	strcpy((*t).name,total_course_names[j]);
	puts((*t).name);
	printf("COURSE EXISTS IN COLLEGE? (1/0)"); scanf("%d",&ch);
	if(ch)
	{
		printf("INPUT RATING OF COURSE(10-1): "); scanf("%d",&((*t).rating));
		printf("INPUT no. of seats: "); scanf("%u",&((*t).no_seats));
		for(i=0; i<marks_criteria; i++)
		{
			printf("INPUT MARKS_CRITERIA %d: MIN. MARKS: ",i+1); scanf("%d",&((*t).min_marks[i]));
		}
	(*t).no=j;
	}
	else
	{
		(*t).no_seats=0;
		(*t).rating=-1;
	}
	fflush(stdin);
}
void college_input(struct college *t)
{
	fflush(stdin);
	printf("INPUT college name: ");
	gets((*t).name);
	int i;
	(*t).no_courses=0;
	for(i=0; i<total_no_courses; i++)
	{
		course_input(&((*t).courses[i]),i);
		if((*t).courses[i].no_seats>0)
			(*t).no_courses++;
	}
	fflush(stdin);
}
void save_college(struct college *t)
{
	FILE *fptr;
	fptr=fopen("colleges.dat","ab");
	if(fptr!=NULL)
		fwrite(t,sizeof(*t),1,fptr);
	else
		printf("FILE CANNOT BE OPENED 1\n");
	fclose(fptr);
}
/*struct course
{
	int rating;
	unsigned int no_seats;
	char name[strsize];
	int min_marks[marks_criteria];
};
struct college
{
	char name[strsize];
	struct course courses[total_no_courses];
	int no_courses;

};*/
void course_display(struct course t)
{
	int i;
	if(t.no_seats>0)
	{
		printf("COURSE NAME: "); puts(t.name);
		printf("\b\tNO. of seats: %u\tRATING: %d\n",t.no_seats,t.rating);
		for(i=0; i<marks_criteria; i++)
			printf("\tMINIMUM MARKS CRITERIA %d: %d\n",i+1,t.min_marks[i]);
	}
}
void college_display(struct college t)
{
	int i;
	printf("COLLEGE NAME: "); puts(t.name);
	for(i=0; i<total_no_courses; i++)
		course_display(t.courses[i]);
}
int college_display2(struct college t, unsigned int j, int marks)
{
	int i;

	if(t.courses[j-1].min_marks[0]<=marks)
        {   printf("COLLEGE NAME: "); puts(t.name);
            course_display(t.courses[j-1]);
            return 1;
        }
    else
        return 0;
}
void add_college()
{
	struct college t;
	printf("INPUT UNIQUE COLLEGE NAMES ONLY\n");//checking for uniqueness not done yet
	college_input(&t);
	save_college(&t);
}
void view_allcolleges()
{
	FILE *fptr;
	fptr=fopen("colleges.dat","rb");
	struct college t;
	if(fptr!=NULL)
	{
		fread(&t,sizeof(t),1,fptr);
		while(!(feof(fptr)))
		{
			college_display(t);
			fread(&t,sizeof(t),1,fptr);
		}
		fclose(fptr);
	}
	else
		printf("FILE CANNOT BE OPENED 2\n");
}
void del_college()
{
	view_allcolleges();
	char nm[strsize];
	struct college t;
	int found=0; fflush(stdin);
	printf("INPUT COLLEGE NAME TO BE DELETED: "); gets(nm); fflush(stdin);
	FILE *fptr1, *fptr2;
	fptr1=fopen("colleges.dat","rb");
	fptr2=fopen("temp.dat","ab");
	fread(&t,sizeof(t),1,fptr1);
	while(!(feof(fptr1)))
	{
		if((strcmpi(nm,t.name)==0)&&(!found))
			found++;
		else
			fwrite(&t,sizeof(t),1,fptr2);
		fread(&t,sizeof(t),1,fptr1);
	}
	fclose(fptr1);
	fclose(fptr2);
	remove("colleges.dat");
	rename("temp.dat","colleges.dat");
	if(!found)
		printf("RECORD NOT FOUND\n");
}
void edit_college()
{
	view_allcolleges();
	char nm[strsize];
	struct college t,t2;
	int found=0; fflush(stdin);
	printf("INPUT COLLEGE NAME TO BE EDITED: "); gets(nm); fflush(stdin);
	FILE *fptr1;
	fptr1=fopen("colleges.dat","rb+");
	fread(&t,sizeof(t),1,fptr1);
	while((!(feof(fptr1)))&&(!found))
	{
		if(strcmpi(nm,t.name)==0)
		{
			found++;
			printf("INPUT CHANGED COLLEGE DETAILS\n");
			college_input(&t2);
			fseek(fptr1,(-1*sizeof(t)),SEEK_CUR);
			fwrite(&t,sizeof(t),1,fptr1);
		}
		fread(&t,sizeof(t),1,fptr1);
	}
	fclose(fptr1);
	if(!found)
		printf("RECORD NOT FOUND\n");
}
void delete_data()
{
    FILE *ptr;
    ptr=fopen("colleges.dat","wb");
    fclose(ptr);
}
void admin()
{
	int ch;
	do
	{
		printf("ADD COLLEGE(1)\tDELETE COLLEGE(2)\tEDIT COLLEGE(3)\tVIEW ALL COLLEGES(4)\tEXIT(5)\tDELETE ALL(6)\n");
		printf("INPUT CHOICE: ");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:	add_college();
					break;
			case 2:	del_college();
					break;
			case 3: edit_college();
					break;
			case 4:	view_allcolleges();
					break;
			case 5:	break;
			case 6: delete_data();
                    break;
			default:printf("invalid input\n");
		}
	}
	while(ch!=5);
}

void student_input(struct student *t)
{
	int i;
	fflush(stdin);
	printf("INPUT NAME: "); gets((*t).name); fflush(stdin);
	printf("INPUT MARKS");
	for(i=0; i<marks_criteria; i++)
		scanf("%u",&((*t).marks[i]));
    fflush(stdin);
	printf("INPUT preference order:");
	for(i=0; i<total_no_courses; i++)
	{
		printf("COURSE NO.: %d", i+1); puts(total_course_names[i]);
	}
	printf("INPUT PREFERENCE ORDER:\n");
	fflush(stdin);
	for(i=0; i<total_no_courses; i++)
		scanf("%d",&((*t).preference[i]));
}
void sort(struct college all[], unsigned int t, int n)
{
	int i,j;
	for(i=0; i<n-1; i++)
	{
		for(j=i+1; j<n; j++)
		{
			if(all[i].courses[t].rating<all[j].courses[t].rating)
			{
				struct college temp=all[i];
				all[i]=all[j];
				all[j]=temp;
			}
		}
	}
}
void view_best_colleges(struct student t)
{
	struct college all_colleges[total_colleges];
	FILE *ptr;
	struct college c;
	int i=0;
	ptr=fopen("colleges.dat","rb");
	fread(&c,sizeof(c),1,ptr);
	while(!(feof(ptr)))
	{
		all_colleges[i]=c;
		i++;
		fread(&c,sizeof(c),1,ptr);
	}
	fclose(ptr);
	i=0;
	sort(all_colleges,(t.preference[0]-1),(i+1));
	int p=0;
	while(p<3)
    {
        p+=college_display2(all_colleges[i],t.preference[0],t.marks[0]);
        i++;
    }

	sort(all_colleges,(t.preference[1]-1),(i+1));
	i=0;p=0;
	while(p<3)
    {
        p+=college_display2(all_colleges[i],t.preference[1],t.marks[0]);
        i++;
    }
    sort(all_colleges,(t.preference[2]-1),(i+1));
    i=0;p=0;
	while(p<3)
    {
        p+=college_display2(all_colleges[i],t.preference[2],t.marks[0]);
        i++;
    }
}
void student()
{
	int ch;
	struct student t;
	do
	{
		printf("INPUT STUDENT DETAILS(1)\tVIEW BEST COLLEGES(2)\tEXIT(3)\n");
		printf("INPUT CHOICE: ");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:	student_input(&t);
					break;
			case 2:	view_best_colleges(t);
					break;
			case 3:	break;
			default:printf("invalid input\n");
		}
	}
	while(ch!=3);
}
void initializer()
{
	int ch;
	do
	{
		printf("ADMIN(1)\tSTUDENT(2)\tEXIT(3)\n");
		printf("INPUT CHOICE: ");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:	admin();
					break;
			case 2:	student();
					break;
			case 3: break;
			default:printf("invalid input\n");
		}
	}
	while(ch!=3);
}
int main()
{
	initializer();
	return 0;
}
