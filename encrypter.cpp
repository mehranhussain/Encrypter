
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <dirent.h>



void printHeader()
{
	cout<<"\n\t +----- Data Encryption Program for DSA Final Project (Cryptography Algorithms) -----+\n\n";
}


class Encrypter
{
protected:

	float filecounter,tempcounter,per;

public:
	Encrypter()
	{
		filecounter=0;tempcounter=0;per=0;
	}

	bool countDirectory(char*);
	bool countFile(char*);
	bool directoryProcess(char*);

	virtual bool processOnIt(char*);
	void rc4Algorithm(unsigned char*, unsigned char*, unsigned char*&);
	
	void perc(void);
	string correctPath(string);
};


void Encrypter::rc4Algorithm(unsigned char* ByteInput, unsigned char* pwd,unsigned char* &ByteOutput)
{

	unsigned char * output;
	int internalkey[256], tempkey[256];

	int i,j=0,t,tmp,tmp2;

	for (tmp=0;tmp<256;tmp++)
	{
		internalkey[tmp]=tmp;
		tempkey[tmp]=pwd[(tmp % strlen((char *)pwd))];
	}

	for (i=0;i<256;i++)
	{
		j = (j + internalkey[i] + tempkey[i]) % 256;

		tmp=internalkey[i];
		internalkey[i]=internalkey[j];
		internalkey[j]=tmp;
	}

	output = new unsigned char [ (int)strlen((char *)ByteInput)  + 1 ] ;
	i=j=0;

	for (tmp=0;tmp<strlen((char *)ByteInput);tmp++)
	{
	    i = (i + 1) % 256;
       j = (j + internalkey[i]) % 256;

		tmp2=internalkey[i];
		internalkey[i]=internalkey[j];
		internalkey[j]=tmp2;
      t = (internalkey[i] + internalkey[j]) % 256;

		if (internalkey[t]==ByteInput[tmp])
			output[tmp]=ByteInput[tmp];
		else
			output[tmp]=internalkey[t]^ByteInput[tmp];
	}

	output[tmp]='\0';
	ByteOutput=output;

}

bool Encrypter::countFile(char* pathE)
{
	fstream file(pathE,ios::binary|ios::out|ios::in);
	if(!file)
	{
		countDirectory(pathE);
		return true;
	}
	filecounter++;
	return true;
}


bool Encrypter::countDirectory(char* pathD)
{
	DIR *dir;
	dirent *ent;
    dir = opendir (pathD);
	if (dir != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			if(ent->d_name[0]!='.'&& ent->d_name[0]!='$')
			{
				string ps=pathD;
				ps.append("\\");
				ps.append(ent->d_name);
 				char* pathD = new char[ps.size() + 1];
            std::copy(ps.begin(), ps.end(), pathD);
            pathD[ps.size()] = '\0';
				countFile(pathD);
			}
 
		}
			closedir (dir);
	} 
	else
	{
      perror ("");
	  return EXIT_FAILURE;
	}
	return true;
}


string Encrypter::correctPath(string p)
{
	int n;
	for(n=0;n<p.length();n++)
	{
		if(p[n]=='\\')
		{
			p.insert(n,1,'\\');
			n++;
		}
	}
	return p;
}


bool Encrypter::processOnIt(char* pathE)
{
	string copy;
	fstream file(pathE,ios::binary|ios::out|ios::in);
	if(!file)
	{
		directoryProcess(pathE);
		return true;
	}
	unsigned char* outData;
	unsigned char* data;
	char inoutData[256];

	

	
	file.read((char*) (inoutData),sizeof(char*));

	data = (unsigned char*) inoutData;

	rc4Algorithm(data,(unsigned char*)"Secret",outData);

	file.seekp(0,ios::beg);
	file.write((char*) (outData),sizeof(char*));


	tempcounter++;
	perc();
	return true;
}


bool Encrypter::directoryProcess(char* pathD)
{
	DIR *dir;
	struct dirent *ent;
    dir = opendir (pathD);
	if (dir != NULL)
	{
		 while ((ent = readdir (dir)) != NULL)
		 {
			if(ent->d_name[0]!='.')
			{
			   string ps=pathD;
			   ps.append("\\\\");
			   ps.append(ent->d_name);
 			   char* pathD = new char[ps.size() + 1];
         	std::copy(ps.begin(), ps.end(), pathD);
   			   pathD[ps.size()] = '\0';
			   processOnIt(pathD);
			}
 
		}
			closedir (dir);
	} 
	else
	{
      perror ("");
	  return EXIT_FAILURE;
	}
	return true;

}

void Encrypter::perc(void)
{
	clrscr();
	per=((tempcounter/filecounter)*100);
	cout<<"\n\n\n\t\t Processing: "<<(int)per<<"%";
}

class TxtEncrypter : public Encrypter
{
public:
	bool processOnIt(char*);

};

bool TxtEncrypter::processOnIt(char* path)
{
	string copy;
	ifstream infile(path,ios::binary|ios::in);
	


	if(!infile)
	{
		directoryProcess(path);
		return true;
	}
	unsigned char* outData;
	unsigned char* data;
	char inoutData[256];

	clrscr();
   printHeader();
	cout<<"\n\n\t\t 1) Encrypt\n\n \t\t 2) Decrypt\n\n\t\t Choice: ";
	char choice;
	cin>>choice;
	if(choice == '1')
	{
		ofstream Eoutfile("encrypt.dsa",ios::binary|ios::out);
		while(infile.read((char*)inoutData,sizeof(char*)))
		{

		data = (unsigned char*) inoutData;

		rc4Algorithm(data,(unsigned char*)"Secret",outData);

		Eoutfile.write((char*) (outData),sizeof(char*));
		}
	}
	else if(choice == '2')
	{
		ofstream outfile("Output.txt",ios::binary|ios::out);
		while(infile.read((char*)inoutData,sizeof(char*)))
		{

			data = (unsigned char*) inoutData;

			rc4Algorithm(data,(unsigned char*)"Secret",outData);

			outfile.write((char*) (outData),sizeof(char*));
		}	
	}

	tempcounter++;
	perc();
	return true;
}

int main()
{
	clrscr();
   Encrypter* obj;
   printHeader();
   cout<<"\n\n\n\t\t 1) Executable Files\n\n \t\t 2) Text Files\n \n\t\t Choice: ";
   char choice;
   cin>>choice;
   if(choice == '2')
   {
      obj = new TxtEncrypter();
   }
   else
   {
   	obj = new Encrypter();
   }

	string path;
   clrscr();
   printHeader();
	cout<<"\n\n\n\n\t\tEnter Your File/Directory Path: ";
	cin>>path;
	path=obj->correctPath(path);
	char* pt = new char[path.size() + 1];
	std::copy(path.begin(), path.end(), pt);
	pt[path.size()] = '\0';
	obj->countFile(pt);
	obj->processOnIt(pt);
   clrscr();
   printHeader();
	cout<<"\n\n\n\n\t\tYour Processing has Done Successfully.. :-)";
	getch();
	return 0;
}
