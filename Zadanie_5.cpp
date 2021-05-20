/* Zadanie_4.cpp
    
    Filip Kurasz 249458
    Sterowanie Procesami Dyskretnymi

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include "Dane.h"

using namespace std;


// usun z vectora o numerze indeksu
void remove_from_vec(vector<Dane>& rhs, int index)
{
    vector<Dane> temp;
    for (int i = 0; i < rhs.size(); i++)
    {
        if (i == index)
            continue;
        temp.push_back(rhs.at(i));
    }
    rhs = temp;
}

// kolejnosc elementow
void order(vector<Dane>& W, Dane e)
{
    int count = 0;
    // zlicz ilosc tych samych elementow
    for (int i = 0; i < W.size(); i++)
    {
        if (e.getIndex() == W.at(i).getIndex())
            count++;
    }
    // jesli mamy wiecej niz 1 ten sam element
    if (count > 1)
    {
        for (int i = 0; i < W.size(); i++)
        {
            if (e.getIndex() == W.at(i).getIndex())
            {
                // to usun pierwszy
                remove_from_vec(W, i);
                break;
            }
        }
    }
}

// oblicza Cmax na podstawie kolejności
int c_max(vector<Dane>& W)
{
    int cmax = 0;
    int t = 0;

    t += W.at(0).getR();

    for (int i = 0; i < W.size(); i++)
    {
        if (W.at(i).getR() > t)
        {
            t = W.at(i).getR();
        }
        t += W.at(i).getP();
        cmax = std::max(cmax,t+W.at(i).getQ());
    }
    
    return cmax;
}

// sortowanie werktora danych po R
void vector_sort_R(vector<Dane>& data)
{
	for (int j = 0; j < data.size() - 1; j++)
	{
		for (int i = 0; i < data.size() - j - 1; i++) {
			if (data[i].R < data[i + 1].R)
			{
				Dane temp = data[i + 1];
				data[i + 1] = data[i];
				data[i] = temp;
			}
		}
	}
}
// sortowanie po Q
void vector_sort_Q(vector<Dane>& data)
{
	for (int j = 0; j < data.size() - 1; j++)
	{
		for (int i = 0; i < data.size() - j - 1; i++) {
			if (data[i].Q > data[i + 1].Q)
			{
				Dane temp = data[i + 1];
				data[i + 1] = data[i];
				data[i] = temp;
			}
		}
	}

}

// na 3.0
void Schrage(vector<Dane> N, int& Cmax, vector<Dane>& W)
{
    int t = 0;
    vector<Dane> G; //kolejka
    Dane e; //aktualny

    while (!(G.size() == 0) || !(N.size() == 0))
    {
        // dodaj do G wszystkie elementy które spełniają R < t i usun je z N
        for (int i = 0; i < N.size(); i++)
        {
            if (N.at(i).getR() <= t)
            {
                e = N.at(i); G.push_back(e); remove_from_vec(N, i);
                i = -1;
            }
        }
        // zbior G jest pusty
        if (G.size() == 0)
        {
            // znajdz najmniejsze mozliwe R i wpisz je jako nowe t
            int min_t = N.at(0).getR();
            for (int i = 1; i < N.size(); i++)
            {
                if (min_t > N.at(i).getR())
                {
                    min_t = N.at(i).getR();
                }
            }
            t = min_t;
            continue;
        }
        // wybierz te zadanie ktore ma najwieksze Q
        int max_q = G.at(0).getQ();
        int index = 0;
        for (int i = 1; i < G.size(); i++)
        {
            if (max_q < G.at(i).getQ())
            {
                max_q = G.at(i).getQ(); index = i;
            }
        }
        e = G.at(index);
        W.push_back(e);
        order(W, e);
        remove_from_vec(G, index);

        // aktualizuje t oraz Cmax
        t += e.getP(); Cmax = std::max(Cmax, t + e.getQ());
    }
}

// wersja 1
void SchragePRMT(vector<Dane> N, int& Cmax)
{
    // krok 1
	int t = 0; 
	vector_sort_R(N); 
	vector<Dane> G;
	// krok 2  
	while (G.size() != 0 || N.size() != 0)
	{
		// krok 2.1  
		while (N.size()!=0 && N.back().R <= t)
		{
			Dane e = N.back();
			G.push_back(e);
			N.pop_back();
		}
		// krok 2.2  
		int t_next;
		if (N.empty()) t_next = 999999999;
		else t_next = N.back().R;
		
		// krok 2.4  
		if (G.empty()) t = t_next;
		// krok 2.5  
		else 	
		{
			// krok 2.5.1  
			vector_sort_Q(G);
			// krok 2.5.2
			if (t_next < t + G.back().P)
			{
				G.back().P = G.back().P - (t_next - t);
				t = t_next;
			}
			// krok 2.5.3  
			else
			{
				t = t + G.back().P;
				Cmax = max(Cmax, G.back().Q+t);
				G.pop_back();
			}
		}
	}
}

// obliczanie bloku
std::tuple<int, int, int> obliczanieBloku(std::vector<Dane> permutacja) // liczenie bloku a, b, c na podstawie permutacji
{
  int a = -1;
  int b = -1;
  int c = -1;

  int time = permutacja[0].R;
  int cMax = 0;
  // CMAX dla bloku
  int CMAX = c_max(permutacja);
  Dane pi(0,0,0,0);

  int n = permutacja.size();

  // idz po wszystkich zadaniach
  for(int i = 0; i < n; i++)
  {
    Dane temp = permutacja[i];

    // wybierz wiekszy czas
    time = std::max(time, temp.R);
    time += temp.P;

    if((time + permutacja[i].Q) == CMAX)
    {
        b = i;//b jako ostatni zwiększa CMAX
    }

    cMax = max(cMax, time + temp.Q);
  }

  time = permutacja[0].R;
  cMax = 0;
  
  // idz po wszystkich zadaniach
  for(int i = 0; i < n; i++)
  {
    Dane temp = permutacja[i];

    // wybierz wiekszy czas
    time = max(time, temp.R);
    time += temp.P;

    {
      if(a < 0)
      {
      int suma = 0;
      for(int j = i; j <= b; j++)
      {
        suma += (permutacja[j].P);
      }
      suma += permutacja[b].Q;
      if(CMAX == permutacja[i].R + suma)
        a = i;
      }
    }

    cMax = max(cMax, time + temp.Q);
  }

    time = permutacja[0].R;
    cMax = 0;

  for(int i = a; i <= b; i++)
  {
    if(permutacja[i].Q < permutacja[b].Q)
        c = i;
  }

  return std::make_tuple(a,b,c);
}


// carlier N_vec[i] to np. data001: , ub to Cmax, W to permutacja
void Carlier(vector<Dane> N,int& ub,vector<Dane>& W)
{
    int cmax_schrage = 0;
    //permutacja z schrage
  std::vector<Dane> PI;
  Schrage(N,cmax_schrage,PI);

    // cmax z schrage_prmt
  int u = 0;
  //eval(PI);//wynik schrage bez
    Schrage(PI,u,PI);

  if(u < ub)
    ub = u;
  //wyznacz blok
  auto bl = obliczanieBloku(PI);
  int b = std::get<1>(bl);
  int c = std::get<2>(bl);

  if(c == -1)
    return;

    // tutaj wybierany jest ten 140 7 67
  int ri = 1000000, qi = 10000000, pi = 0;
  for(int i = c + 1; i <= b; i++)
  {
    if(PI[i].R < ri)
      ri = PI[i].R;

    if(PI[i].Q < qi)
      qi = PI[i].Q;

    pi += PI[i].P;
  }

  int r_temp = PI[c].R;
  PI[c].R = max(PI[c].R, ri + pi);
  int lb = 0;
  SchragePRMT(PI,lb);

  if(lb < ub)
    Carlier(PI,ub,PI);

  PI[c].R = r_temp;

  int q_temp = PI[c].Q;
  PI[c].Q = max(PI[c].Q, qi + pi);
  //lb = schrage_prmtS(PI);
  SchragePRMT(PI,lb);

  if(lb < ub)
    Carlier(PI,ub,PI);

  PI[c].Q = q_temp;
  W = PI;
}

// dodaje 7 elementow ze slajdu
void add(vector<vector<Dane>> &n)
{
    vector<Dane> temp;
    temp.push_back(Dane(0,27,78,1));
    temp.push_back(Dane(140,7,67,2));
    temp.push_back(Dane(14,36,54,3));
    temp.push_back(Dane(133,76,5,4));
    
    /*
    temp.push_back(Dane(77,11,82,1));
    temp.push_back(Dane(34,92,8,2));
    temp.push_back(Dane(88,36,30,3));
    temp.push_back(Dane(1,98,9,4));
    */
    n.push_back(temp);
}
// dodaje dane z pliku
void addFromFile(string path, vector<vector<Dane>> &n)
{
    ifstream file(path);
    string discard;
    while (getline(file, discard))
    {
        if (discard.find("data.000:") != std::string::npos)
            break;
        else
            continue;
    }

    int amount = 0;
    int r = 0;
    int p = 0;
    int q = 0;
    

    int count = 0;

    while (!file.eof())
    {
        if (file >> amount)
        {
            count++;
            vector<Dane> temp_vec;
            for (int i = 0; i < amount; i++)
            {
                file >> r >> p >> q;
                temp_vec.push_back(Dane(r, p, q, i+1));
            }
            n.push_back(temp_vec);
        }
        else
        {
            //file nie wczytal danych lub dane sa bledne (czyszczenie strumienia i porzucenie znakow)
            //np. dla napisu "data.1" w pliku 
            file.clear();
            file.ignore(numeric_limits<streamsize>::max(), '\n');

            while (getline(file, discard))
            {
                string data_numer = "data.00" + to_string(count) + ":";
                if (discard.find(data_numer) != std::string::npos)
                    break;
                else
                    continue;
            }
        }
    }
    file.close();
}



int main()
{
    // Cmax
    int Cmax = 0;
    int CmaxSum = 0;
    // vector vectorow danych
    vector<vector<Dane>> N_vec;
    // nieuporządkowany zbiór
    vector<Dane> N;

    // kolejnosc
    vector<Dane> W;
    vector<vector<Dane>> W_vec;

    // DODANIE DANYCH
    //add(N_vec);
    addFromFile("data.txt",N_vec);

    std::cout << "SCHRAGE" << endl;
    // Schrage Z WYNIKAMI 
    //std::cout << "Rozmiar: " << N_vec.size() << endl;
    for (int i = 0; i < N_vec.size(); i++)
    {
        W.clear();
        Cmax = 0;
        // uporządkowany zbior Dla danych z data.001 ...itp.
        Schrage(N_vec[i], Cmax, W);
        W_vec.push_back(W);

        std::cout << "data.00" << i << ": ";
        std::cout << "Cmax: " << Cmax << " ";
        //std::cout << "Cmax: " << c_max(W) << " ";
        std::cout << "Kolejnosc: ";
        for (int j = 0; j < W.size(); j++)
        {
            std::cout << W.at(j).getIndex() << " ";
        }
        std::cout << endl;
    }
    std::cout << endl;

    std::cout << "SCHRAGE PRMT" << endl;
    // Schrage Z WYNIKAMI 
    //std::cout << "Rozmiar: " << N_vec.size() << endl;
    for (int i = 0; i < N_vec.size(); i++)
    {
        W.clear();
        Cmax = 0;
        // uporządkowany zbior Dla danych z data.001 ...itp.
        SchragePRMT(N_vec[i], Cmax);
        W_vec.push_back(W);

        std::cout << "data.00" << i << ": ";
        std::cout << "Cmax: " << Cmax << endl;
    }
    std::cout << endl << endl;

    std::cout << "CARLIER" << endl;
    for (int i = 0; i < N_vec.size(); i++)
    {
        W.clear();
        Cmax = 999999999;
        // uporządkowany zbior Dla danych z data.001 ...itp.
        //Schrage(N_vec[i], Cmax, W);
        Carlier(N_vec[i],Cmax,W);
        W_vec.push_back(W);

        std::cout << "data.00" << i << ": ";
        std::cout << "Cmax: " << Cmax << " ";
        //std::cout << "Cmax: " << c_max(W) << " ";
        std::cout << "Kolejnosc: ";
        for (int j = 0; j < W.size(); j++)
        {
            std::cout << W.at(j).getIndex() << " ";
        }
        std::cout << endl;
    }
    std::cout << endl << endl;

}

