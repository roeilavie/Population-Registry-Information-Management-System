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

void print_person(const Person* citizen);//������ �� �� ����� �����

void add_person(db_mgr* citizen);// ������� ������� ������

void insertPerson(Person* newPerson, db_mgr* citizen); // �������� ��� �����

long long checkID(long long id); //�������� ��� ����� �.�

void checkExists(Person* newPerson, const db_mgr* citizen); //�������� ��� ����� �� ��.� ����

void checkBirth(Person* newPerson);// �������� ��� ����� �� ������ ���� ����

void checkPartner(Person* newPerson);// �������� ��� ����� �� ���

void checkMother(Person* newPerson);// �������� ��� ����� ���

void checkFather(Person* newPerson);// �������� ��� ����� ���

void checkChildren(Person* newPerson);// �������� ��� ����� �����

void init_db(db_mgr* p); // ���� ���� ������ ����� �������

char menu(); // ���� ������ ������

void changearr(const db_mgr* citizen);// ������ ��� ���� 

Person* search_id(const db_mgr* citizen, long long id); //����� �.�

void search_person(const db_mgr* citizen); // ����� �����

void search_parents(const db_mgr* citizen);//����� �����

void delete_person(db_mgr* citizen);// ����� ����� ������

void Move(db_mgr* citizen, long long tmp); // �������� ��� ������ ������ ������

void deleteChild(Person* family, long long deleteId); //����� ���� �� ���� 

void get_gen(db_mgr* citizen); //����� �����

void print_db(const db_mgr* citizen); //������ ���� ������

void search_by_name(const db_mgr* citizen); //����� ������� ��

void quit(db_mgr* citizen);// ����� �������

int get_left_len(db_mgr* citizen, Person* p);// ����� �� ��� ������ ������� ����� ���� ��� ��� �� ������� ���� ������

Person_LVL* pop(Person_LVL** arr, int* size);// ����� ���� �� ����� ������, ������ ���� ��� �� ���.

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

void print_person(const Person* citizen) // ����� �� ������� �� �������
{
	printf("Name: %s\t", citizen->name);
	printf("Surname: %s\t", citizen->familyname);
	printf("ID: %llu\t", citizen->ID);
	printf("birth: %d-%d-%d\n", citizen->birth.day, citizen->birth.month, citizen->birth.year);
	printf("Partner ID: %llu\t", citizen->partnerID);
	printf("Mother ID: %llu\t", citizen->motherID);
	printf("Father ID: %llu\n", citizen->fatherID);

	if (citizen->NumofChildren)// �� ������ ����� ����� �� ������ ����� ����
	{
		for (int i = 0; i < citizen->NumofChildren; i++)
		{
			printf("children id: #%d ID is: %llu\n", (i + 1), citizen->ChildrenPtr[i]);
		}
	}
	printf("\n");
}

void init_db(db_mgr* p) // ���� ���� ������ ����� �������
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

char menu() // ���� ������ ������
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

void add_person(db_mgr* citizen)// ������� ������� ������
{
	db_mgr tmpCitizen;
	tmpCitizen.actualcitizen = citizen->actualcitizen;
	init_db(&tmpCitizen);
	Person tempPerson;

	if (citizen->realTimeCitizen > citizen->actualcitizen) //����� ������� ����� ��� ���� ���� ������ ���� ����� ����
	{
		for (int i = 0; i < citizen->realTimeCitizen - 1; i++)
		{
			tmpCitizen.arrofcitizen[i] = citizen->arrofcitizen[i];// ���� ���� ����� ���� ���� ��� ����� �� ������
		}

		if (citizen->realTimeCitizen > 1)
		{
			free(citizen->arrofcitizen);// �� �� ���� ������ ����� ���� � 1 �� ���� ������ ��� ����� ������ ����.
		}
		citizen->actualcitizen++; //�� ��� ��� ���� ���� ������� �����
		init_db(citizen);//  ����� ���� ���� ���� ++

		if (citizen->actualcitizen > 1) // ���� ������ ����� �� ����� ���� ���� ���� ����� ���� ���� 
		{
			for (int i = 0; i < citizen->realTimeCitizen; i++)//������� �� �� ������ ���� ����
			{
				citizen->arrofcitizen[i] = tmpCitizen.arrofcitizen[i];
			}
		}

		citizen->realTimeCitizen = citizen->actualcitizen; // ����� �� ���� ������ ����� ����� ������� �����
		insertPerson(&tempPerson, citizen);// ����� ��������� INSERTPERSON ������ ���� �� �� ���� ������
		citizen->arrofcitizen[(citizen->actualcitizen) - 1] = tempPerson; // ���� ���� ��� ��� ����� ���� ����� �� ������
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

void insertPerson(Person* newPerson, db_mgr* citizen) // �������� ��� �����
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

	checkExists(newPerson, citizen);// ����� �� �.� ����
	checkBirth(newPerson);// ����� ������ ����� ���� ����
	checkPartner(newPerson);// ����� ������ �.� �� ��� ����
	checkMother(newPerson);// ����� ������ �.� ��� ����
	checkFather(newPerson);// ����� ������ �.� ��� ����
	checkChildren(newPerson);// ����� �� �� �����, ������ �� �.� ����
}

void checkExists(Person* newPerson, const db_mgr* citizen)//�������� ��� ����� �� ��.� ����
{
	int i;
	do {
		printf("Please enter your ID: ");
		scanf("%llu", &(newPerson->ID));
		newPerson->ID = checkID(newPerson->ID); //����� ������ ����� �����
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

void checkBirth(Person* newPerson)// �������� ��� ����� �� ������ ���� ����

{
	int day, month;
	do //����� ������ ����� ����
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

void checkPartner(Person* newPerson)// �������� ��� ����� �� ���
{
	int tmp;
	do { //����� ������� ���� �����
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

void checkMother(Person* newPerson)// �������� ��� ����� ���
{
	int tmp;
	do { //����� ������� ���� �����
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

void checkFather(Person* newPerson)// �������� ��� ����� ���
{
	int tmp;
	do { //����� ������� ���� �����
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

void checkChildren(Person* newPerson)// �������� ��� ����� �����
{
	int tmp, i, j;
	do { // ����� ������ ���� ����� ����
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
		newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]); //����� �� ����� ����� �����

		while (newPerson->ChildrenPtr[i] == newPerson->ID) //����� ���� �� ���� �� ��� ����
		{
			printf("This id already taken!\nPlease enter the ID for the #%d child: ", i + 1);
			scanf("%llu", &(newPerson->ChildrenPtr[i]));
			newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]);
		}

		for (j = 0; j < i; j++)
		{
			while (newPerson->ChildrenPtr[j] == newPerson->ChildrenPtr[i])// ����� ���� �� ���� �� ��� ��� ������
			{
				printf("This id already taken!\nPlease enter the ID for the #%d child: ", i + 1);
				scanf("%llu", &(newPerson->ChildrenPtr[i]));
				newPerson->ChildrenPtr[i] = checkID(newPerson->ChildrenPtr[i]);
			}
		}
	}
}

long long checkID(long long id)//�������� ��� ����� �.�
{
	while (id <= 0)
	{
		printf("wrong input, please try again\n");
		scanf("%llu", &id);
	}
	return id;
}

void changearr(const db_mgr* citizen) // ������ ��� ���� 
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

Person* search_id(const db_mgr* citizen, long long id) //����� �.�
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

void search_person(const db_mgr* citizen) // ����� �����
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

void search_parents(const db_mgr* citizen) //����� �����
{
	long long tmp;
	printf("please enter ID:\n");
	scanf("%llu", &tmp);
	Person* tmpPerson = search_id(citizen, tmp);
	Person* mom;
	Person* dad;

	if (tmpPerson != NULL)
	{
		mom = search_id(citizen, tmpPerson->motherID);// ���� ����� �� ���
		dad = search_id(citizen, tmpPerson->fatherID);// ���� ����� �� ���
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

void delete_person(db_mgr* citizen)// ������ ����� �� ����� ���� ������
{
	long long tmp;
	printf("please enter ID:\n");
	scanf("%llu", &tmp);// ������ ����� ���� �� ���� ��� ���� �����
	Person* ptr = search_id(citizen, tmp);// ������ �� ������ �� ���� ���� ����� �����
	Person* partner;
	Person* child;

	if (!ptr)
	{
		printf("wrong id!\n");
		return;
	}

	if (ptr->partnerID)// ������ �� �� �� �����
	{
		partner = search_id(citizen, ptr->partnerID);
		partner->partnerID = 0;// �� �� ������ ���� � 0
	}

	if (ptr->fatherID) // ����� ��� ���� ��
	{
		deleteChild(search_id(citizen, ptr->fatherID), ptr->ID);//  ����� ������ �� ���� �� �.� �� ����
	}

	if (ptr->motherID)// ����� ��� ����� ��
	{
		deleteChild(search_id(citizen, ptr->motherID), ptr->ID);// ����� ������ �� ���� �� �.� �� ���� 
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

	Move(citizen, tmp); // �������� ��� ������ �� ��� ��� ������� ���� ��� ��� ���.
}

void Move(db_mgr* citizen, long long tmp) // �������� ���
{
	int j = 0;
	db_mgr tempCitizen;
	if (!search_id(citizen, tmp))
	{
		printf("No citizen found\n");
		return;
	}
	tempCitizen.actualcitizen = (citizen->actualcitizen) - 1;// ������� ����� ���� �� ���� ����� ���� ��� 
	init_db(&tempCitizen);// ���� ����� ����� ������ �������

	for (int i = 0; i < citizen->actualcitizen; i++)
	{
		if (citizen->arrofcitizen[i].ID != tmp) // �� ��� ����� ����� �� ����� ���� ���� ����� ����� ������, ������ �� ������
		{
			if (citizen->arrofcitizen[i].ID)
			{
				tempCitizen.arrofcitizen[j] = citizen->arrofcitizen[i];
				j++;
			}
		}
	}

	free(citizen->arrofcitizen);// ����� ����� �� ������ �����
	tempCitizen.realTimeCitizen = citizen->realTimeCitizen - 1;// ����� ���� ������ �����
	*citizen = tempCitizen;// ����� �� �� ������ 
	citizen->arrofcitizen = tempCitizen.arrofcitizen;// ���� ����� ����� ������
	tempCitizen.arrofcitizen = NULL;// ������ ������ �����
	printf("Deletion complete!\n");
	return;
}

void deleteChild(Person* family, long long deleteId) // ������ �� ������ �� ����/��� + ����� ����� �� ����/���. 
{
	Person tempfamily;
	int j = 0;

	if (!family)// ����� �� ���� ��� ���
	{
		printf("people not found\n");
		return;
	}

	tempfamily.NumofChildren = (int)family->NumofChildren - 1;// ����� ���� ���� �� ���� ������ ���� ������
	if (!(int)tempfamily.NumofChildren)// ����� �� ���� ���� ���� ���
	{
		family->ChildrenPtr = NULL;
		family->NumofChildren = 0;
		return;
	}

	tempfamily.ChildrenPtr = (long long*)malloc((int)tempfamily.NumofChildren * sizeof(long long)); //����� ���� ����� ����� ������ ������
	if (!tempfamily.ChildrenPtr)
	{
		printf("out of memory");
		return;
	}

	for (int i = 0; i < (int)family->NumofChildren; i++)// ���� ���� ������ ����� ���� ��� ���� ����� �����
	{
		if (family->ChildrenPtr[i] != deleteId)
		{
			tempfamily.ChildrenPtr[j] = family->ChildrenPtr[i];
			j++;
		}
	}
	free(family->ChildrenPtr);// ����� ���� ������ ����

	family->ChildrenPtr = (long long*)malloc((int)tempfamily.NumofChildren * sizeof(long long));// ����� ���� ����� ������ ������
	if (!family->ChildrenPtr)
	{
		printf("out of memory");
		return;
	}
	family->NumofChildren = (int)tempfamily.NumofChildren;

	for (int i = 0; i < (int)family->NumofChildren; i++)// ���� �� ���� ������ ����
	{
		family->ChildrenPtr[i] = tempfamily.ChildrenPtr[i];
	}
	free(tempfamily.ChildrenPtr);// ����� ���� ������ �����
}

void get_gen(db_mgr* citizen) //����� �����
{
	long long tmp;
	int curr_lvl = 0;// ���� �� ����� �������
	Person* tmpPerson;
	Person_LVL* childrentmp;// ����� ����� ���� ����� ��� ����� ���� ������ ����
	printf("please enter ID:\n");
	scanf("%llu", &tmp);
	tmpPerson = search_id(citizen, tmp); // ���� ����� �� ��� ���

	if (!tmpPerson)
	{
		printf("the person is not found!\n");
		return;
	}

	curr_lvl++;
	int max_lvl = curr_lvl, len = (int)tmpPerson->NumofChildren;// ����� ����� ��������� �LEN  ���� ����� ������ ��� ����
	childrentmp = (Person_LVL*)malloc(len * sizeof(Person_LVL));//����� ����� ������
	if (!childrentmp)
	{
		printf("out of memory");
		return;
	}

	if (!len)
	{
		printf("The generation is : 1\n");// �� ��� ����� �� ���� ��� ���� 1
		free(childrentmp);
		return;
	}

	if (len > 0) {
		curr_lvl++;// �� �� ����� ���� �� ����� � 1 
		max_lvl = curr_lvl;// ����� �� ������ ��������� ����� �������
	}

	for (int i = 0; i < len; i++)
	{
		Person_LVL child;
		child.person_id = tmpPerson->ChildrenPtr[i];// ���� ���� ������ ����� �� ���� ����� � I
		child.lvl = curr_lvl;
		childrentmp[i] = child;// ����� �� ������ ( ���� ������ ������ ����) ����� ���� ������
	}

	while (len > 0)
	{
		Person_LVL* temp = pop(&childrentmp, &len);// ����� ��������-���� ���� �������� 
		curr_lvl = temp->lvl;// ���� �� ����� �������
		long long child_id = temp->person_id;
		Person* curr_child = search_id(citizen, child_id);//( ���� ������ �� ���� ������ �� ��� ���� ������ ���� ���� �� �������� ������ �� ����� ����� �� ���� ����)

		if ((int)curr_child->NumofChildren == 0)
		{
			continue;// �� �� ������ ����� ���� �� ����� ���� ��� 
		}

		curr_lvl++;// ������ ����� ���� ����� �� ����� � 1 
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

int get_left_len(db_mgr* citizen, Person* p) // ����� ����� ����� �� �� ������
{
	int max = 0;
	if (p->NumofChildren == 0)// ����� ��� ������ ����� ������� 0
	{
		return max;
	}

	max += 1;// ���� ���, �� ���� �� ������ ����� �1
	Person* child = search_id(citizen, p->ChildrenPtr[0]); // ���� ����� �� ����

	while ((int)child->NumofChildren > 0)//  ����� �� ������ ����� �1 �� ��� �� ���� ��� ����
	{
		max++;
		child = search_id(citizen, child->ChildrenPtr[0]);
	}
	return max;
}

Person_LVL* pop(Person_LVL** arr, int* size)// ����� ���� �� ������ ������ ����� �� ����� ��� �����
{
	Person_LVL res = (*arr)[0];// ���� ����� ������ ����� � CHILDRENTEMP
	for (int i = 0; i < *size - 1; i++)// ���� ������ �� ������ ������ �� ���� �� ���� ������
	{
		(*arr)[i] = (*arr)[i + 1];
	}
	(*size)--;// ���� ������� �� ���� ��  ���� ������ 
	return &res;// ����� ������ �� ���� �� ���� ������ ����� �� �����. 
}

void append(Person_LVL** arr, long long* ids, int* arrsize, int childrenCount, int lvl)
{
	Person_LVL* res = (Person_LVL*)malloc((*arrsize + childrenCount) * sizeof(Person_LVL));// ���� ���� ����� �� LEN ���� ���� ������ �� ����.
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

void print_db(const db_mgr* citizen) //������ database
{
	if (!(citizen->realTimeCitizen))
	{
		printf("no more people!\n");
		return;
	}

	if (citizen->realTimeCitizen > 1) // �� �� ���� ��� ��� ��� ���� ����� �����
	{
		changearr(citizen);
	}


	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		printf("**person details #%d**\n", i + 1);
		print_person(&(citizen->arrofcitizen[i]));
	}
}

void search_by_name(const db_mgr* citizen) //����� �� �� ��� ����� �� ���� ��
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

	changearr(citizen);// ����� ������ �� ���� ��� ��� ���� ����� �����
	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		if ((!(strcmp(surname, citizen->arrofcitizen[i].familyname)) && !(strcmp(name, citizen->arrofcitizen[i].name))))// ����� ��� ��� ���� ���� ����� ����
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

void quit(db_mgr* citizen)// ����� �������
{
	printf("the program is done\n");
	for (int i = 0; i < citizen->realTimeCitizen; i++)
	{
		free(citizen->arrofcitizen[i].name);
		free(citizen->arrofcitizen[i].familyname);
		free(citizen->arrofcitizen[i].ChildrenPtr);
	}
}

