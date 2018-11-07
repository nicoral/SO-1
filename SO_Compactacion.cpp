#include <iostream>
#include <vector>
#include <utility>

using namespace std;


struct process
{
	string nom;
	int Tmemoria;
	int tama;
	bool band;
	process(string n,int Tiem,int tam)
	{
		this->nom=n;
		this->Tmemoria=Tiem;
		this->tama=tam;
		this->band=false;
	}
};
struct particiones
{
	string nom;
	int tam;
	int tiempo;
	bool estado;
};
struct memoria
{
	int tamMax;
	vector<particiones> parti;
	memoria(int t)
	{
		this->tamMax=t;
	}
};
bool huecos(vector<particiones> &A)
{
	for(int i=0;i<A.size()-1;i++)
	{
		if(A[i].nom==" ")
		{
			return true;
		}
	}
	return false;
}
int compactar(vector<particiones> &A)
{
	int ncompactados=0;
	while(huecos(A))
	{
		for(int i=0;i<A.size();i++)
		{
			if(A[i].nom==" " && A[i].estado==false)
			{
				ncompactados++;
				A.erase(A.begin()+i);
			}
		}
	}
	return ncompactados;
}
int EspacioLibre(memoria &A)
{
	int es=0;
	for(int i=0;i<A.parti.size();i++)
	{
		es+=A.parti[i].tam;
	}
	return A.tamMax-es;
}
void borrarProcess(string a,vector<process> &B)
{
	for(int i=0;i<B.size();i++)
	{
		if(B[i].nom==a)
		{
			B.erase(B.begin()+i);
			break;
		}
	}
}
bool procesoTerminado(memoria &M)
{
	for(int i=0;i<M.parti.size();i++)
	{
		if(M.parti[i].tiempo==0)
			return true;
	}
	return false;
}
void imprimirM(memoria &M,int tem)
{
	for(int i=0;i<M.parti.size();i++)
	{
		cout<<M.parti[i].nom<<" "<<M.parti[i].tam<<" "<<M.parti[i].tiempo<<endl;
	}
	cout<<"Espacio Libre:"<<EspacioLibre(M)<<endl;
	cout<<"tiempo:"<<tem<<endl;
	cout<<""<<endl;
}


bool primer_ajuste(vector<process> &A,memoria &M,int &i){
	for(int j=0;j<M.parti.size();j++)
	{
		if(M.parti[j].estado==false && M.parti[j].tam>=A[i].tama)
		{
			M.parti[j].nom=A[i].nom;
			M.parti[j].tiempo=A[i].Tmemoria;
			M.parti[j].estado=true;
			A[i].band=true;
			return true;
		}
	}
	return false;
}

bool mejor_ajuste(vector<process> &A,memoria &Memoria,int &i){
	int size=A[i].tama;
	int menor=6969,pos=-1;
	for(int j=0;j<Memoria.parti.size();j++)
	{
		//evaluando particiones libres
		if(Memoria.parti[j].estado==false && Memoria.parti[j].tam>=A[i].tama )
			if(abs(size-Memoria.parti[j].tam)<menor){
				menor=abs(size-Memoria.parti[j].tam);
				pos=j;
			}
	}
	if(pos!=-1){
		Memoria.parti[pos].nom=A[i].nom;
		Memoria.parti[pos].tiempo=A[i].Tmemoria;
		Memoria.parti[pos].estado=true;
		A[i].band=true;
		return true;
	}
	else return false;
}

bool peor_ajuste(vector<process> &A,memoria &Memoria,int &i){
	int size=A[i].tama;
	int mayor=-6969,pos=-1;
	for(int j=0;j<Memoria.parti.size();j++)
	{
		//evaluando particiones libres
		if(Memoria.parti[j].estado==false && Memoria.parti[j].tam>=A[i].tama )
			if(abs(size-Memoria.parti[j].tam)>mayor){
				mayor=abs(size-Memoria.parti[j].tam);
				pos=j;
			}
	}
	if(pos!=-1){
		Memoria.parti[pos].nom=A[i].nom;
		Memoria.parti[pos].tiempo=A[i].Tmemoria;
		Memoria.parti[pos].estado=true;
		A[i].band=true;
		return true;
	}
	else return false;

}
bool siguiente_ajuste(vector<process> &A,memoria &Memoria,int &i,int &sig)
{
	if(sig==Memoria.parti.size()-1)
	{
		return primer_ajuste(A,Memoria,i);
	}
	else
	{
		for(int j=sig;j<Memoria.parti.size();j++)
		{
			if(Memoria.parti[j].estado==false && Memoria.parti[j].tam>=A[i].tama )
			{
				Memoria.parti[j].nom=A[i].nom;
				Memoria.parti[j].tiempo=A[i].Tmemoria;
				Memoria.parti[j].estado=true;
				A[i].band=true;
				sig=j;
				return true;
			}
		}
		for(int j=0;j<sig;j++)
		{
			if(Memoria.parti[j].estado==false && Memoria.parti[j].tam>=A[i].tama )
			{
				Memoria.parti[j].nom=A[i].nom;
				Memoria.parti[j].tiempo=A[i].Tmemoria;
				Memoria.parti[j].estado=true;
				A[i].band=true;
				sig=j;
				return true;
			}
		}
		return false;
	}
}
bool colocacion(vector<process> &A,memoria &Memoria,int &i,int ajuste,int &sig)
{
    if(ajuste==1){return primer_ajuste(A,Memoria,i);}
    if(ajuste==2){return mejor_ajuste(A,Memoria,i);}
    if(ajuste==3){return peor_ajuste(A,Memoria,i);}
    if(ajuste==4){return siguiente_ajuste(A,Memoria,i,sig);}
}


void Pcompactacion(vector<process> &A,int mem,int ajuste)
{
	memoria M(mem);
	int tem=0,sig=0;
	particiones auxP;
	int cont=0;
	//mientras que hayan procesos
	while(!A.empty() && cont<10)
	{
		cont++;
		//metiendo los procesos a la memoria
		for(int i=0;i<A.size();i++)
		{
			//si es el primer proceso
			if(M.parti.empty())
			{
				if(A[i].tama<EspacioLibre(M) && A[i].band==false)
				{
					A[i].band=true;
					auxP.nom=A[i].nom;
					auxP.tam=A[i].tama;
					auxP.tiempo=A[i].Tmemoria;
					auxP.estado=true;
					M.parti.push_back(auxP);
					cont=0;
					sig=M.parti.size()-1;
				}
			}
			//si ya hay procesos existentes en memoria
			else
			{
				if(A[i].band==false)
				{
					if(colocacion(A,M,i,ajuste,sig))
	                	cont=0;
				}
				if(A[i].tama<EspacioLibre(M) && A[i].band==false)
				{
					A[i].band=true;
					auxP.nom=A[i].nom;
					auxP.tam=A[i].tama;
					auxP.tiempo=A[i].Tmemoria;
					auxP.estado=true;
					M.parti.push_back(auxP);
					cont=0;
					sig=M.parti.size()-1;
				}

			}
		}
		imprimirM(M,tem);
		tem++;
		for(int i=0;i<M.parti.size();i++)
		{
			if(M.parti[i].tiempo==0)
			{
				for(int j=i+1;j<M.parti.size();j++)
				{
					M.parti[j].tiempo-=1;
				}
				for(int j=0;j<M.parti.size();j++)
				{
					if(M.parti[j].tiempo==0)
					{
						borrarProcess(M.parti[j].nom,A);
						M.parti[j].nom=" ";
						M.parti[j].estado=false;
					}
				}
				
				///ban=false;
				/**cout<<"1°: "<<endl;
				imprimirM(M,tem);*/
				break;
			}
			else
			{
				M.parti[i].tiempo-=1;
			}
		}
		cout<<"2°: "<<endl;
		imprimirM(M,tem);
	}
}

int main()
{
	vector<process> v;
	process aux("p1",2,100);
	v.push_back(aux);
	process aux1("p2",1,50);
	v.push_back(aux1);
	process aux2("p3",2,40);
	v.push_back(aux2);
	process aux3("p4",1,20);
	v.push_back(aux3);
	process aux4("p5",2,40);
	v.push_back(aux4);
	Pcompactacion(v,200,4);
	return 0;
}
