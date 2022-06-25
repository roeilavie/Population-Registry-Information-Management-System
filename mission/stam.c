#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 50

typedef struct
{
	int year;
	char month;
	char day;
}DateofBirth;

typedef struct
{
	char* name;
	char* familyname;
	long long ID;
	long long partnerID, motherID, fatherID;
	char NumofChildren;
	long long* ChildrenPtr;
	DateofBirth birth;
}Person;

typedef struct
{
	Person* arrofcitizen;
	int actualcitizen;
	int realTimeCitizen;
}db_mgr;

typedef struct 
{
	long long person_id;
	int lvl;
}Person_LVL;

void print_person(const Person* citizen);//מדפיסה את כל נתוני התושב

void add_person(db_mgr* citizen);// פונקציה המוסיפה תושבים

void insertPerson(Person* newPerson, db_mgr* citizen); // פונקציית עזר קליטה

long long checkID(long long id); //פונקציית עזר בודקת ת.ז

void checkExists(Person* newPerson, const db_mgr* citizen); //פונקציית עזר בודקת אם הת.ז תפוס

void checkBirth(Person* newPerson);// פונקציית עזר בודקת אם התאריך לידה תקין

void checkPartner(Person* newPerson);// פונקציית עזר בדיקה בן זוג

void checkMother(Person* newPerson);// פונקציית עזר בדיקה אמא

void checkFather(Person* newPerson);// פונקציית עזר בדיקה אבא

void checkChildren(Person* newPerson);// פונקציית עזר בדיקה ילדים

void init_db(db_mgr* p); // מקצה מערך דינאמי למספר התושבים

char menu(); // הצגת התפריט למשתמש

void changearr(const db_mgr* citizen);// מחליפה לפי הסדר 

Person* search_id(const db_mgr* citizen, long long id); //מחפשת ת.ז

void search_person(const db_mgr* citizen); // מחפשת אנשים

void search_parents(const db_mgr* citizen);//מחפשת הורים

void delete_person(db_mgr* citizen);// מוחקת אנשים מהמערך

void Move(db_mgr* citizen, long long tmp); // פונקציית עזר למחיקה האנשים מהמערך

void deleteChild(Person* family, long long deleteId); //מחיקת הילד אם נמצא 

void get_gen(db_mgr* citizen); //מחשבת דורות

void print_db(const db_mgr* citizen); //מדפיסה בסיס נתונים

void search_by_name(const db_mgr* citizen); //חיפוש באמצעות שם

void quit(db_mgr* citizen);// יציאה מהתפריט

int get_left_len(db_mgr* citizen, Person* p);// מקבלת את שני הזוגות הסדורים וממפה אותם אחד אחד עד שמגיעים לילד האחרון

Person_LVL* pop(Person_LVL** arr, int* size);// מקבלת מערך של ילדים וגודלו, ומקבלת מערך חדש כל פעם.

void append(Person_LVL** arr, long long* ids, int* arrsize, int idsize, int lvl);

void main()
{

	db_mgr manager;
	manager.realTimeCitizen = 0;
	char decision;
	printf("Please enter how many citizens\n");
	scanf("%d", &(manager.actualcitizen));
	init_db(&(manager));


	do {
		decision = menu();
		switch (decision)
		{
		case 1:
			manager.realTimeCitizen++;
			add_person(&(manager));
			break;

		case 2:
			search_person(&(manager));
			break;

		case 3:
			search_parents(&(manager));
			break;

		case 4:
			delete_person(&(manager));
			break;

		case 5:
			get_gen(&(manager));
			break;

		case 6:
			print_db(&(manager));
			break;

		case 7:
			search_by_name(&(manager));
			break;

		case 8:
			quit(&(manager));
			break;
		}

	} while (decision != 8);

	system("pause");
}

void print_person(const Person* citizen) // הדפסת כל הנתונים של האזרחים
{
	printf("Name: %s\t", citizen->name);
	printf("Surname: %s\t", citizen->familyname);
	printf("ID: %llu\t", citizen->ID);
	printf("birth: %d-%d-%d\n", citizen->birth.day, citizen->birth.month, citizen->birth.year);
	printf("Partner ID: %llu\t", citizen->partnerID);
	printf("Mother ID: %llu\t", citizen->motherID);
	printf("Father ID: %llu\n", citizen->fatherID);

	if (citizen->NumofChildren)// אם קיימים ילדים נדפיס את תעודות הזהות שלהם
	{
		for (int i = 0; i < citizen->NumofChildren; i++)
		{
			printf("children id: #%d ID is: %llu\n", (i + 1), citizen->ChildrenPtr[i]);
		}
	}
	printf("\n");
}

void init_db(db_mgr* p) // מקצה מערך דינאמי למספר התושבים
{
	if (p->actualcitizen)
	{
		p->arrofcitizen = (Person*)calloc(p->actualcitizen, sizeof(Person));

		if (!p->arrofcitizen)
		{
			printf("Out of memory");
			return;
		}
	}
}

char menu() // הצגת התפריט למשתמש
{
	int decision;
	char call;
	do
	{
		printf("Database system Menu:\n1. Add person\n2. Search a person\n3. Search Parents\n4. Delete a person\n5. Get generation\n6. Print database\n7. Search by name\n8. Quit\n");
		scanf("%d", &decision);
		call = (char)decision;
		if ((int)call < 1 || (int)call >8)
		{
			printf("Error\n");
		}
	} while ((int)call < 1 && (int)call >8);
	return (int)call;
}

void add_person(db_mgr* citizen)// פונקציה המוסיפה תושבים
{
	db_mgr tmpCitizen;
	tmpCitizen.actualcitizen = citizen->actualcitizen;
	init_db(&tmpCitizen);
	Person tempPerson;

	if (citizen->realTimeCitizen > citizen->actualcitizen) //במידה והמשתמש הכניס קלט גדול יותר מהמערך נבצע הקצאה חדשה
	{
		for (int i = 0; i < citizen->realTimeCitizen - 1; i++)
		{
			tmpCitizen.arrofcitizen[i] = citizen->arrofcitizen[i];// נבצע השמה למבנה מנהל החדש כדי לשמור את הערכים
		}

		if (citizen->realTimeCitizen > 1)
		{
			free(citizen->arrofcitizen);// רק אם מספר האנשים בפועל גדול מ 1 אז נבצע שיחרור וכך ניפטר מהמערך הישן.
		}
		citizen->actualcitizen++; //בן אדם אחד נוסף מחוץ לגבולות המערך
		init_db(citizen);//  הקצאת מקום נוסף לאחר ++

		if (citizen->actualcitizen > 1) // לאחר ההקצאה נעתיק את המבנה מנהל החדש לתוך המבנה מנהל הישן 
		{
			for (int i = 0; i < citizen->realTimeCitizen; i++)//מעתיקים את כל הערכים שהיו לפני
			{
				citizen->arrofcitizen[i] = tmpCitizen.arrofcitizen[i];
			}
		}

		citizen->realTimeCitizen = citizen->actualcitizen; // נשווה את מספר האנשים בפועל למספר האזרחים הכולל
		insertPerson(&tempPerson, citizen);// קריאה לפונקציית INSERTPERSON המבקשת כקלט את כל פרטי המשתמש
		citizen->arrofcitizen[(citizen->actualcitizen) - 1] = tempPerson; // נבצע השמה לבן אדם שנקלט לתוך המערך של האנשים
	}

	else
	{
		insertPerson(&tempPerson, citizen);
		citizen->arrofcitizen[(citizen->realTimeCitizen) - 1] = tempPerson;
	}

	if (citizen->realTimeCitizen > 1)
	{
		free(tmpCitizen.arrofcitizen);
	}
}

void insertPerson(Person* newPerson, db_mgr* citizen) // פונקציית עזר קליטה
{
	char name[SIZE];
	char surname[SIZE];
	printf("Please enter the First name\n");
	scanf(" %s", name);
	fseek(stdin, 0, SEEK_END);
	int size = strlen(name);
	newPerson->name = (char*)malloc((size + 1) * sizeof(char));
	if (!newPerson->name)
	{
		printf("Out of memory");
		return;
	}
	strcpy(newPerson->name, name);


	printf("Please enter the Surname\n");
	scanf(" %s", surname);
	fseek(stdin, 0, SEEK_END);
	size = strlen(surname);
	newPerson->familyname = (char*)malloc((size + 1) * sizeof(char));
	if (!newPerson->familyname)
	{
		printf("Out of memory");
		return;
	}
	strcpy(newPerson->familyname, surname);

	checkExists(newPerson, citizen);// בדיקה אם ת.ז קיים
	checkBirth(newPerson);// בדיקה שהוכנס תאריך לידה תקין
	checkPartner(newPerson);// בדיקה שהוכנס ת.ז בן זוג תקין
	checkMother(newPerson);// בדיקה שהוכנס ת.ז אמא תקין
	checkFather(newPerson);// בדיקה שהוכנס ת.ז אבא תקין
	checkChildren(newPerson);// בדיקה אם יש ילדים, ובדיקה אם ת.ז תקין
}

void checkExists(Person* newPerson, const db_mgr* citizen)//פונקציית עזר בודקת אם הת.ז תפוס
{
	int i;
	do {
		printf("Please enter your ID: ");
		scanf("%llu", &(newPerson->ID));
		newPerson->ID = checkID(newPerson->ID); //בדיקה שתעודת הזהות תקינה
		if (citizen->actualcitizen > 1)
		{
			for (i = 0; i < citizen->actualcitizen; i++)
			{
				if (citizen->arrofcitizen[i].ID == newPerson->ID)
				{

					printf("this ID is already taken, please try again!\n");
					break;
				}
			}
		}
		else
			break;
	} while (citizen->arrofcitizen[i].ID == newPerson->ID);
}

void checkBirth(Person* newPerson)// פונקציית עזר בודקת אם התאריך לידה תקין

{
	int day, month;
	do //בדיקה שתאריך הלידה תקין
	{
		printf("Please enter your date of birth order by dd-mm-yy\n");
		fseek(stdin, 0, SEEK_END);
		scanf("%d-%d-%d", &day, &month, &(newPerson->birth.year));
		newPerson->birth.day = (char)day;
		newPerson->birth.month = (char)month;
		if (((int)newPerson->birth.day < 1) || ((int)newPerson->birth.day > 30) || (((int)newPerson->birth.month < 1) || ((int)newPerson->birth.month > 12) || newPerson->birth.year < 0 || !newPerson->birth.year))
		{
			printf("Error\n");
		}
	} while (((int)newPerson->birth.day < 1) || ((int)newPerson->birth.day > 30) || (((int)newPerson->birth.month < 1) || ((int)newPerson->birth.month > 12)) || (newPerson->birth.year < 0 || !newPerson->birth.year));
}

void checkPartner(Person* newPerson)// פונקציית עזר בדיקה בן זוג
{
	int tmp;
	do { //בדיקה שהתעודת זהות תקינה
		printf("Do you have partner press 1->yes, 0->no\n");
		scanf("%d", &tmp);
		switch (tmp)
		{
		case 1:
			printf("Please enter your Partner ID: ");
			scanf("%llu", &(newPerson->partnerID));
			newPerson->partnerID = checkID(newPerson->partnerID);
			break;

		case 0:
			newPerson->partnerID = 0;
			break;

		default:
			printf("wrong input! please try again\n");
			break;
		}
	} while ((tmp != 0) && (tmp != 1));
}

void checkMother(Person* newPerson)// פונקציית עזר בדיקה אמא
{
	int tmp;
	do { //בדיקה שהתעודת זהות תקינה
		printf("Do you have mother press 1->yes, 0->no\n");
		scanf("%d", &tmp);
		switch (tmp)
		{
		case 1:
			printf("Please enter your mother ID: ");
			scanf("%llu", &(newPerson->motherID));
			newPerson->motherID = checkID(newPerson->motherID);
			break;

		case 0:
			newPerson->motherID = 0;
			break;

		default:
			printf("wrong input! please try again\n");
			break;
		}
	} while ((tmp != 0) && (tmp != 1));
}

void checkFather(Person* newPerson)// פונקציית עזר בדיקה אבא
{
	int tmp;
	do { //בדיקה שהתעודת זהות תקינה
		printf("Do you have father press 1->yes, 0->no\n");
		scanf("%d", &tmp);
		switch (tmp)
		{
		case 1:
			printf("Please enter your father ID: ");
			scanf("%llu", &(newPerson->fatherID));
			newPerson->fatherID = checkID(newPerson->fatherID);
			break;

		case 0:
			newPerson->fatherID = 0;
			break;

		default:
			printf("wrong input! please try again\n");
			break;
		}
	} while ((tmp != 0) && (tmp != 1));
}

void checkChildren(Person* newPerson)// פונקציית עזר בדיקה ילדים
{
	int tmp, i, j;
	do { // בדיקה שהוכנס מספר ילדים תקין
		printf("Do you have children press 1->yes, 0->no\n");
		scanf("%d", &tmp);
		switch (tmp)
		{
		case 1:
			printf("Enter how many children you have: ");
			scanf("%d", &(newPerson->NumofChildren));
			fseek(stdin, 0, SEEK_END);
			break;

		case 0:
			newPerson->NumofChildren = 0;
			newPerson->ChildrenPtr = NULL;
			break;

		default:
			printf("wrong input! please try again\n");
			break;
		}
	} while ((tmp != 0) && (tmp != 1));

	newPerson->ChildrenPtr = (long long*)malloc(((int)newPerson->NumofChildren) * sizeof(long long));
	if (!newPerson->ChildrenPtr)
	{
		printf("Out of memory");
		return;
	}

	for (i = 0; i < (int)newPerson->NumofChildren; i++)
	{
		printf("Please enter the ID for the #%d child: ", i + 1);
		scanf("%llu", &(newPerson->ChildrenPtr[i]));
		newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]); //בדיקה אם תעודת הזהות תקינה

		while (newPerson->ChildrenPtr[i] == newPerson->ID) //בדיקה שהתז לא תפוס על ידי האבא
		{
			printf("This id already taken!\nPlease enter the ID for the #%d child: ", i + 1);
			scanf("%llu", &(newPerson->ChildrenPtr[i]));
			newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]);
		}

		for (j = 0; j < i; j++)
		{
			while (newPerson->ChildrenPtr[j] == newPerson->ChildrenPtr[i])// בדיקה שהתז לא תפוס על ידי אחד הילדים
			{
				printf("This id already taken!\nPlease enter the ID for the #%d child: ", i + 1);
				scanf("%llu", &(newPerson->ChildrenPtr[i]));
				newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]);
			}
		}
	}
}

long long checkID(long long id)//פונקציית עזר בודקת ת.ז
{
	while (id <= 0)
	{
		printf("wrong input, please try again\n");
		scanf("%llu", &id);
	}
	return id;
}

void changearr(const db_mgr* citizen) // מחליפה לפי הסדר 
{
	Person tempPerson;
	int j = 0;
	while (j != citizen->realTimeCitizen)
	{
		for (int i = 0; i < citizen->realTimeCitizen; i++)
		{
			if (citizen->arrofcitizen[i + 1].ID)
			{
				if (citizen->arrofcitizen[i].ID > citizen->arrofcitizen[i + 1].ID)
				{
					tempPerson = citizen->arrofcitizen[i];
					citizen->arrofcitizen[i] = citizen->arrofcitizen[i + 1];
					citizen->arrofcitizen[i + 1] = tempPerson;
				}
			}

		}
		j++;
	}
}

Person* search_id(const db_mgr* citizen, long long id) //מחפשת ת.ז
{
	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		if (citizen->arrofcitizen[i].ID == id)
		{
			return (&(citizen->arrofcitizen[i]));
		}
	}
	return NULL;
}

void search_person(const db_mgr* citizen) // מחפשת אנשים
{
	long long tmp;
	printf("please enter ID:\n");
	scanf("%llu", &tmp);

	if (search_id(citizen, tmp))
	{
		printf("**Person details**\n");
		print_person(search_id(citizen, tmp));
	}

	else
		printf("ID not found\n");

}

void search_parents(const db_mgr* citizen) //מחפשת הורים
{
	long long tmp;
	printf("please enter ID:\n");
	scanf("%llu", &tmp);
	Person* tmpPerson = search_id(citizen, tmp);
	Person* mom;
	Person* dad;

	if (tmpPerson != NULL)
	{
		mom = search_id(citizen, tmpPerson->motherID);// קבלת כתובת של האם
		dad = search_id(citizen, tmpPerson->fatherID);// קבלת כתובת של האב
		if (tmpPerson->motherID != NULL)
		{
			printf("**Mom details**\n");
			print_person(mom);
		}

		if (tmpPerson->fatherID != NULL)
		{
			printf("**Dad details**\n");
			print_person(dad);
		}

		if ((!tmpPerson->motherID) && (!tmpPerson->fatherID))
		{
			printf("**parents not found**\n");
		}
	}

	else
		printf("ID not found\n");

}

void delete_person(db_mgr* citizen)// קיבלנו כתובת של המבנה מנהל נתונים
{
	long long tmp;
	printf("please enter ID:\n");
	scanf("%llu", &tmp);// קיבלנו תעודת זהות של האדם אשר נרצה למחוק
	Person* ptr = search_id(citizen, tmp);// הצבענו על הכתובת של האדם שאנו רוצים למחוק
	Person* partner;
	Person* child;

	if (!ptr)
	{
		printf("wrong id!\n");
		return;
	}

	if (ptr->partnerID)// בודקים אם יש לו פרטנר
	{
		partner = search_id(citizen, ptr->partnerID);
		partner->partnerID = 0;// אם יש הפרטנר עובר ל 0
	}

	if (ptr->fatherID) // בדיקה האם קיים אב
	{
		deleteChild(search_id(citizen, ptr->fatherID), ptr->ID);//  שליחת הכתובת של האבא עם ת.ז של הילד
	}

	if (ptr->motherID)// בדיקה האם קיימת אם
	{
		deleteChild(search_id(citizen, ptr->motherID), ptr->ID);// שליחת הכתובת של האמא עם ת.ז של הילד 
	}

	if ((int)ptr->NumofChildren != 0)
	{
		for (int i = 0; i < (int)ptr->NumofChildren; i++)
		{
			child = search_id(citizen, ptr->ChildrenPtr[i]);
			if (child)
			{
				if (child->motherID == tmp)
				{
					child->motherID = 0;
				}

				if (child->fatherID == tmp)
				{
					child->fatherID = 0;
				}
			}

			else
			{
				break;
			}
		}
	}

	Move(citizen, tmp); // פונקציית עזר שמוחקת את הבן אדם ומחזירה מבנה ללא הבן אדם.
}

void Move(db_mgr* citizen, long long tmp) // פונקציית עזר
{
	int j = 0;
	db_mgr tempCitizen;
	if (!search_id(citizen, tmp))
	{
		printf("No citizen found\n");
		return;
	}
	tempCitizen.actualcitizen = (citizen->actualcitizen) - 1;// מגדירים משתנה זמני עם מספר אנשים פחות אחד 
	init_db(&tempCitizen);// נבצע הקצאה למספר האנשים העדכנית

	for (int i = 0; i < citizen->actualcitizen; i++)
	{
		if (citizen->arrofcitizen[i].ID != tmp) // כל עוד תעודת הזהות של התושב אינה שווה לתושב הרצוי למחיקה, להעתיק את נתוניו
		{
			if (citizen->arrofcitizen[i].ID)
			{
				tempCitizen.arrofcitizen[j] = citizen->arrofcitizen[i];
				j++;
			}
		}
	}

	free(citizen->arrofcitizen);// שחרור המערך של האנשים הקודם
	tempCitizen.realTimeCitizen = citizen->realTimeCitizen - 1;// עדכון מספר האנשים בפועל
	*citizen = tempCitizen;// העתקה של כל הפרטים 
	citizen->arrofcitizen = tempCitizen.arrofcitizen;// קבלת כתובת המערך העדכני
	tempCitizen.arrofcitizen = NULL;// נפטרים מהמערך הקודם
	printf("Deletion complete!\n");
	return;
}

void deleteChild(Person* family, long long deleteId) // קיבלנו את הכתובת של האבא/אמא + תעודת הזהות של האבא/אמא. 
{
	Person tempfamily;
	int j = 0;

	if (!family)// בדיקה אם קיים הבן אדם
	{
		printf("people not found\n");
		return;
	}

	tempfamily.NumofChildren = (int)family->NumofChildren - 1;// משתנה זמני מקבל את מספר הילדים אחרי המחיקה
	if (!(int)tempfamily.NumofChildren)// בדיקה אם קיים יותר מילד אחד
	{
		family->ChildrenPtr = NULL;
		family->NumofChildren = 0;
		return;
	}

	tempfamily.ChildrenPtr = (long long*)malloc((int)tempfamily.NumofChildren * sizeof(long long)); //מקצים מקום בהתאם למספר הילדים העדכני
	if (!tempfamily.ChildrenPtr)
	{
		printf("out of memory");
		return;
	}

	for (int i = 0; i < (int)family->NumofChildren; i++)// נבצע השמה למשתנה הזמני החדש ללא הילד שנרצה למחוק
	{
		if (family->ChildrenPtr[i] != deleteId)
		{
			tempfamily.ChildrenPtr[j] = family->ChildrenPtr[i];
			j++;
		}
	}
	free(family->ChildrenPtr);// שחרור מערך הילדים הישן

	family->ChildrenPtr = (long long*)malloc((int)tempfamily.NumofChildren * sizeof(long long));// הקצאת מקום כמספר הילדים העדכני
	if (!family->ChildrenPtr)
	{
		printf("out of memory");
		return;
	}
	family->NumofChildren = (int)tempfamily.NumofChildren;

	for (int i = 0; i < (int)family->NumofChildren; i++)// השמה של מערך הילדים החדש
	{
		family->ChildrenPtr[i] = tempfamily.ChildrenPtr[i];
	}
	free(tempfamily.ChildrenPtr);// שחרור מערך הילדים הזמני
}

void get_gen(db_mgr* citizen) //מחשבת דורות
{
	long long tmp;
	int curr_lvl = 0;// סופר את הדרגה העדכנית
	Person* tmpPerson;
	Person_LVL* childrentmp;// מצביע למבנה מסוג פרסון לבל המכיל דרגה ותעודת זהות
	printf("please enter ID:\n");
	scanf("%llu", &tmp);
	tmpPerson = search_id(citizen, tmp); // קבלת כתובת של הבן אדם

	if (!tmpPerson)
	{
		printf("the person is not found!\n");
		return;
	}

	curr_lvl++;
	int max_lvl = curr_lvl, len = (int)tmpPerson->NumofChildren;// ספירת הדרגה המקסימלית וLEN  שווה לכמות הילדים שיש לאדם
	childrentmp = (Person_LVL*)malloc(len * sizeof(Person_LVL));//הקצאה כמספר הילדים
	if (!childrentmp)
	{
		printf("out of memory");
		return;
	}

	if (!len)
	{
		printf("The generation is : 1\n");// אם אין ילדים אז הדור הוא פשוט 1
		free(childrentmp);
		return;
	}

	if (len > 0) {
		curr_lvl++;// אם יש ילדים נעלה את הדרגה ב 1 
		max_lvl = curr_lvl;// נשווה את הספירה המקסימלית לדרגה העדכנית
	}

	for (int i = 0; i < len; i++)
	{
		Person_LVL child;
		child.person_id = tmpPerson->ChildrenPtr[i];// נבצע השמה לתעודת הזהות של הילד במקום ה I
		child.lvl = curr_lvl;
		childrentmp[i] = child;// הוספה של הילדים ( דרגה עדכנית ותעודת זהות) למערך החדש שיצרנו
	}

	while (len > 0)
	{
		Person_LVL* temp = pop(&childrentmp, &len);// קריאה לפונקציה-הסבר בתוך הפונקציה 
		curr_lvl = temp->lvl;// השמה של הדרגה העדכנית
		long long child_id = temp->person_id;
		Person* curr_child = search_id(citizen, child_id);//( קבלת הכתובת של הילד הראשון כל פעם הילד הראשון יהיה שונה כי בפונקציה שקראנו לה למעלה דורסת את הערך שהיה)

		if ((int)curr_child->NumofChildren == 0)
		{
			continue;// אם לא קיימים ילדים לילד אז נעבור לילד הבא 
		}

		curr_lvl++;// קימיים ילדים ולכן נגדיל את הדרגה ב 1 
		append(&childrentmp, curr_child->ChildrenPtr, &len, (int)curr_child->NumofChildren, curr_lvl);
		curr_lvl += get_left_len(citizen, curr_child) - 1;

		if (curr_lvl > max_lvl)
		{
			max_lvl = curr_lvl;
		}
	}
	printf("The generation is : %d\n", max_lvl);
	free(childrentmp);
}

int get_left_len(db_mgr* citizen, Person* p) // בדיקת מסלול שמאלי של עץ הדורות
{
	int max = 0;
	if (p->NumofChildren == 0)// במידה ולא קיימים ילדים מחזירים 0
	{
		return max;
	}

	max += 1;// קיים ילד, אז נקדם את המשתנה הזמני ב1
	Person* child = search_id(citizen, p->ChildrenPtr[0]); // קבלת כתובת של הילד

	while ((int)child->NumofChildren > 0)//  מעלים את המשתנה הזמני ב1 כל עוד לא קיים ילד לילד
	{
		max++;
		child = search_id(citizen, child->ChildrenPtr[0]);
	}
	return max;
}

Person_LVL* pop(Person_LVL** arr, int* size)// מקבלת מערך של הילדים שיצרנו שמכיל תז ודרגה ואת גודלו
{
	Person_LVL res = (*arr)[0];// שווה לאיבר הראשון במערך ה CHILDRENTEMP
	for (int i = 0; i < *size - 1; i++)// נרוץ בלולאה על הילדים ונדרוס את הערך של הילד הראשון
	{
		(*arr)[i] = (*arr)[i + 1];
	}
	(*size)--;// לאחר שרדרסנו את הערך של  הילד הראשון 
	return &res;// החזרת הכתובת של הערך של הילד הראשון שכולל תז ודרגה. 
}

void append(Person_LVL** arr, long long* ids, int* arrsize, int childrenCount, int lvl)
{
	Person_LVL* res = (Person_LVL*)malloc((*arrsize + childrenCount) * sizeof(Person_LVL));// מקצה מקום באורך של LEN ועוד כמות הילדים של הילד.
	if (!res)
	{
		printf("out of memory");
		return;
	}

	for (int i = 0; i < *arrsize; i++)
	{
		res[i] = (*arr)[i];
	}


	for (int i = 0, j = *arrsize; i < childrenCount; i++, ++j)
	{
		Person_LVL p;
		p.lvl = lvl;
		p.person_id = ids[i];
		res[j] = p;
	}
	*arrsize += childrenCount;
	*arr = res; 
}

void print_db(const db_mgr* citizen) //מדפיסה database
{
	if (!(citizen->realTimeCitizen))
	{
		printf("no more people!\n");
		return;
	}

	if (citizen->realTimeCitizen > 1) // אם יש יותר מבן אדם אחד נסדר מהקטן לגדול
	{
		changearr(citizen);
	}


	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		printf("**person details #%d**\n", i + 1);
		print_person(&(citizen->arrofcitizen[i]));
	}
}

void search_by_name(const db_mgr* citizen) //בודקת אם יש כמה אנשים עם אותו שם
{
	char name[SIZE];
	char surname[SIZE];
	int count = 0;
	fseek(stdin, 0, SEEK_END);

	printf("enter first name:\n");
	scanf(" %s", name);
	fseek(stdin, 0, SEEK_END);

	printf("enter surname:\n");
	scanf(" %s", surname);
	fseek(stdin, 0, SEEK_END);

	changearr(citizen);// סידור האנשים עם אותו השם לפי הסדר מהקטן לגדול
	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		if ((!(strcmp(surname, citizen->arrofcitizen[i].familyname)) && !(strcmp(name, citizen->arrofcitizen[i].name))))// בדיקה האם השם פרטי והשם משפחה זהים
		{
			printf("**person details **\n");
			print_person(&(citizen->arrofcitizen[i]));
			count++;
		}
	}
	if (count == 0)
	{
		printf("this person is not exists\n");
	}

}

void quit(db_mgr* citizen)// יציאה מהתפריט
{
	printf("the program is done\n");
	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		free(citizen->arrofcitizen[i].name);
		free(citizen->arrofcitizen[i].familyname);
		free(citizen->arrofcitizen[i].ChildrenPtr);
	}
}

