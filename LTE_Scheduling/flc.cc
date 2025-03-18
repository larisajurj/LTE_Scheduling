#include <string.h>
#include <omnetpp.h>
#include <set>
#include <string>
#include <iostream>
using namespace std;

#define MAX_NR_TERMS 16
#define MAX_NR_INP 20
using namespace omnetpp;
#define ev EV

class Vector{
public:
	//constructor
	Vector(int m)
		: _m(m)
	{
		//cout<<"Constructing a vector of "<<_m<<" elements\n";
		vect = new int[m];
	}
	//destructor
	~Vector ()
	{
		//cout<<"Vector is destroyed\n";
		delete [] vect;
	}
	int get_m() const
	{
		return _m;
	}
	int get(int j) const
	{
		return vect[j];
	}
	void set(int pos, int val)
	{
		vect[pos]=val;
	}
	void print()
	{
		for(int i =0;i<_m;i++)
			cout<<vect[i]<<" ";
		cout<<endl;
	}
	//function that mins 2 vectors
	void min_vect(Vector* v2, Vector* result);

private:
	int _m;
	int* vect;
};

void Vector::min_vect(Vector* v2, Vector* result)
{
	if (v2->_m != this->_m) cout<<"ERROR!!! vectors have different sizes\n";
	for(int i=0; i< this->_m; i++)
	{
		result->vect[i] = (this->vect[i] <= v2->vect[i]) ? this->vect[i] : v2->vect[i];
	}
}

class Matrix{
public:
	//constructor
	 Matrix (int m, int n)
	 : _m(m), _n(n)
	 {
		 //cout<<"Constructing a matrix of "<<m<<" lines and "<<n<<" columns \n";
		 mat = new int* [_m];
		 for(int i=0; i<_m ; i++)
			 mat[i] = new int[_n];
	 }
	 //destructor
	 ~Matrix ()
	 {
		 for (int i=0;i<_m;i++)
			delete [] mat[i];
		 delete [] mat;
		 //cout<<"Matrix is destroyed\n";
	 }
	int get_m() const
	{
		return _m;
	}
	int get_n() const
	{
		return _n;
	}
	void set(int i, int j, int val)
	{
		mat[i][j] = val;
	}
	int get(int i, int j) const
	{
		return mat[i][j];
	}
	void print()
	{
		for(int i=0;i<_m;i++)
		{
			for(int j=0;j<_n;j++) cout<<mat[i][j]<<" ";
			cout<<endl;
		}

	}
	//function that determines the maximum column of a matrix
	void max_column(Vector* v);
	//function that determines the minimum of 2 matrices
	void min_matrix(Matrix* m2, Matrix* result);
	//function that determines the maximum of 2 matrices
	void max_matrix(Matrix* m2, Matrix* result);
	//function that mins a matrix with a column vector
	void min_vector(Vector* v, Matrix* result);

private:
	int _m;
	int _n;
	int** mat;
};


void Matrix::min_matrix(Matrix* m2, Matrix* result)
{
	//cout<<"min matrix>>\n";
	if (this->_m != m2->_m || this->_n != m2->_n) cout<<"ERROR!!! matrices have different dimensions\n";
	for(int i=0; i<this->_m;i++)
		for(int j=0; j<this->_n;j++)
			result->mat[i][j] = (this->mat[i][j] <= m2->mat[i][j]) ? this->mat[i][j] : m2->mat[i][j];
}

void Matrix::max_matrix(Matrix* m2, Matrix* result)
{
	//cout<<"max matrix\n";
	if (this->_m != m2->_m || this->_n != m2->_n) cout<<"ERROR!!! matrices have different dimensions\n";
	for(int i=0; i<this->_m;i++)
		for(int j=0; j<this->_n;j++)
			result->mat[i][j] = (this->mat[i][j] >= m2->mat[i][j]) ? this->mat[i][j] : m2->mat[i][j];
}

void Matrix::max_column(Vector* v)
{
	int i;
	//cout<<"max column\n";
	if (this->_m != v->get_m()) cout<<"ERROR!!!dimensions not matching\n";
	for(i=0;i<this->_m;i++)
		v->set(i, 0);

	for(i=0;i<this->_n;i++)
		for(int j=0;j<this->_m; j++)
			v->set(j, (this->mat[j][i] >= v->get(j)) ? this->mat[j][i] : v->get(j));
}

void Matrix::min_vector(Vector* v, Matrix* result)
{
int i,j;
//cout<<"min vector\n";
for(i=0;i<this->_n;i++)
	for(j=0;j<this->_m;j++)
		result->mat[j][i] = (v->get(j) <= this->mat[j][i]) ? v->get(j) : this->mat[j][i];
}

class Term{
public:
	Term(int a, int b, int c, int d, string name)
		:_a(a), _b(b), _c(c), _d(d), _name(name)
	{
		//cout<<"Creating term "<< _a <<" "<< _b<<" "<<_c<<" "<<_d<<" "<<_name<<"  \n";
	}
	~Term()
	{
		//cout<<"Destroying term\n";
	}
	string getName() const
	{
		return _name;
	}
	int getTermRep(int m, int n, Matrix* mat);

private:
	string _name;
	int _a, _b, _c, _d;
};

int Term::getTermRep(int m, int n, Matrix* mat)
{
	int i, j;

	for(i=0;i<m;i++)
	 for(j=0;j<n;j++)
		 mat->set(i,j,0);

	if (_a<0 ||_a> n-1||_b<0 ||_b>n-1||_c<0||_c>n-1||_d<0||_d>n-1)
	{
		cout<<"invalid term\n";
		return -1;
	}

	if (_a>_b||_b>_c||_c>_d)
	{
		cout<<"coordinates are not ordered\n";
		return -1;
	}

	if (_a==_b)
	{
	 for(i=0;i<m;i++)
		 mat->set(i,_a,1);
	}
	else
	{
	float slope = (float) m / (float)(_b-_a);
	for (i=_a;i<=_b;i++)
	{
		int y = floor( slope * (i-_a));
		y = m-y;
		for (j=0;j<y;j++)
			mat->set(j,i,0);
		for (;j<m;j++)
			mat->set(j,i,1);
	}
 }

 if (_b==_c)
 {
	for(i=0;i<m;i++)
		 mat->set(i,_b,1);
 }
 else
 {
	 for (i=_b;i<=_c;i++)
		 for(j=0;j<m;j++)
			 mat->set(j,i,1);
 }
 if (_c==_d)
 {
	 	for(i=0;i<m;i++)
			mat->set(i,_c,1);
 }
 else
 {
	float slope = (float) m / (float)(_d-_c);
	for (i=_c;i<=_d;i++)
	{
		int y = floor( slope * (_d-i));
		y = m-y;
		for (j=0;j<y;j++)
			mat->set(j,i,0);
		for (;j<m;j++)
			mat->set(j,i,1);
	}
  }

	return 0;
}

class FLC : public cSimpleModule
{
  public:
	~FLC()
	{
	int i,j;
	for(i=0; i< MAX_NR_INP+1; i++)
		for(j=0;j<MAX_NR_TERMS;j++)
			delete m_functions[i][j];

	for(i=0;i<n_rules;i++)
		delete [] rules[i];

	delete [] rules;
	}
	
	int fuzzy_inference(int* inp, int nb_inp, int delta);
	
	
  private:
	int bits_n; /* number of binary vectors representing a membership function */
	int bits_m; /* number of bits in each vector */
	int n_inp;  /* number of inputs */
	int n_rules;  /* number of rules */
	int nb_terms[MAX_NR_INP+1];
	Term* m_functions[MAX_NR_INP+1][MAX_NR_TERMS]; /* membership functions for input and output*/
	int** rules;
	cOutVector qtime,qtimew;
	cOutVector res_dep, result_dep;
	Matrix* fuzzify(int crisp_in, int delta);
	int defuzify(Matrix* mat);
	double scale(double n, double m, double a, double b, double x);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);
};


Define_Module(FLC);

void FLC::initialize()
{
    qtime.setName("delay vector");
    qtimew.setName("weight vector");
    result_dep.setName("result nescalat");
    res_dep.setName("res scalat");

   cXMLElement *rootelement = par("config").xmlValue();
   EV <<"initialize FLC\n";
   cXMLElement *current = rootelement->getElementByPath("./n_bits");

   bits_n = atoi(current->getFirstChild()->getNodeValue());
   ev << "bits n:" << bits_n << ".\n";
	
   current = rootelement->getElementByPath("./m_bits");

   bits_m = atoi(current->getFirstChild()->getNodeValue());
   ev << "bits m:" << bits_m << ".\n";

   current = rootelement->getElementByPath("./n_inp");

   n_inp = atoi(current->getFirstChild()->getNodeValue());
   if (n_inp > MAX_NR_INP)
	   ev<<"error: too many inputs\n";

   ev << "nb inputs:" << n_inp << ".\n";

   current = rootelement->getElementByPath("./n_rules");
   n_rules = atoi(current->getFirstChild()->getNodeValue());
   ev<< "nb rules:" <<n_rules << ".\n";

   //read fuzzy sets for inputs and output
	cXMLElementList list = rootelement->getElementsByTagName("fuzzy_set");

	int i_input = 0;
	for (cXMLElementList::iterator it = list.begin(); it != list.end(); it++)
	{
		cXMLElement* el = *it;
		ev<< "fuzzy set type, number of terms "<< el->getAttribute("type")<<","<<el->getAttribute("nterms")<<"\n";
		nb_terms[i_input] = atoi(el->getAttribute("nterms"));
		/* error situation */
		if (nb_terms[i_input] > MAX_NR_TERMS)
			ev<<"error : too many terms\n";
		//read terms
		cXMLElement* term = el->getFirstChild();

		int x[4];
		string name;
		for(int i_term=0;i_term<nb_terms[i_input];i_term++)
		{
		  cXMLElement* et = term->getFirstChild();
		  name = et->getNodeValue();
			ev<<"term name:"<<name<<"\n";
			for(int i=0;i<4;i++)
			{ 
				et = et->getNextSibling();
				x[i] = atoi(et->getNodeValue());
				ev<<"x coordinate:"<<x[i]<<"\n";
			}
		Term* t = new Term(x[0], x[1], x[2], x[3], name);
		m_functions[i_input][i_term] = t;
		term = term->getNextSibling();
		}
		i_input++;
	}

	//read rules
	rules = new int* [n_rules];
	for(int k=0; k <n_rules; k++)
		rules[k] = new int[n_inp + 1];

	list = rootelement->getElementsByTagName("rule");
	int i_rule = 0;
	for (cXMLElementList::iterator it = list.begin(); it != list.end(); it++)
	{
		cXMLElement* el = *it;
		cXMLElement* ef = el->getFirstChild();
		string term_name;

		//read premises
		for(int i=0;ef!=NULL && i<n_inp;i++, ef = ef->getNextSibling())
		{
			term_name = ef->getNodeValue();
			ev<<"premise: "<< term_name;	
			//find the index of the term in premise
			int found = 0;
			for(int j=0;j<nb_terms[i];j++)
			{
				Term* t = m_functions[i][j];
				if (term_name.compare(t->getName()) == 0)
				{
					found = 1;
					rules[i_rule][i] = j;  /* remember index of term */
					//cout<< "rules["<<i_rule<<"]["<<i<<"]="<<j<<"\n";
				}
			}
			if (found == 0) cout<<"ERROR: term name(premise) not found\n";

		}
		//read conclusion
		int i=n_inp;
		ev<<"conclusion: "<<ef->getNodeValue()<<"\n";
		term_name = ef->getNodeValue();
		int found = 0;
		for(int j=0;j<nb_terms[n_inp];j++)
			{
				Term* t = m_functions[i][j];
				if (term_name.compare(t->getName()) == 0)
				{
					found = 1;
					rules[i_rule][n_inp] = j;
				}
			}
		if (found == 0) cout<<"ERROR: term name(conclusion) not found\n";

		i_rule++;
	}
}


Matrix* FLC::fuzzify(int crisp_in, int delta)
{
	int i, j;
	Matrix* matrix = new Matrix(this->bits_m, this->bits_n);

	ev << "Fuzzify input\n";
	for(i=0;i<this->bits_m;i++)
		for(j=0;j<this->bits_n;j++)
			matrix->set(i, j, 0);
	
	if (crisp_in<0||crisp_in>this->bits_n-1)
	{
		printf("invalid input\n");
		return NULL;
	}

    if (delta == 0)
	{
		for(i=0;i<this->bits_m;i++)
			matrix->set(i, crisp_in, 1);
		return matrix;
	}

	float slope = (float) this->bits_m / (float) delta;
	for (i=0;i<delta;i++)
	{
		int y = floor((delta-i) * slope);
		y = this->bits_m-y;
		if (crisp_in+i < this->bits_n)
		{
		for(j=0;j<y;j++)
			matrix->set(j, crisp_in+i, 0);
		for(;j<this->bits_m;j++)
			matrix->set(j, crisp_in+i, 1);
		}
		if (crisp_in-i >= 0)
		{
		for(j=0;j<y;j++)
			matrix->set(j, crisp_in-i, 0);
		for(;j<this->bits_m;j++)
			matrix->set(j, crisp_in-i, 1);
		
		}
	}

	return matrix;
}


int FLC::defuzify(Matrix* mat)
{
 int result, suma, val, i,j;
 result = suma = 0;

 for(i=bits_n-1;i>=0;i--)
 {
	 val = 0;
	 for(j=0;j<bits_m;j++)
		 val += mat->get(j,i);
	 suma += val;
	 result += suma;
 }

 if (suma != 0)
	 return result/suma;
 else return 0;
}

double FLC::scale(double n, double m, double a, double b, double x)
{
  double res;
  if((b<=a)||(m<=0))    
    exit(1);
   else if(x<=a)
    res=n;
   else if (x>=b) 
    res=m;
   else 
    res= ((x-a)/(b-a)) * (m-n) + n;
  return res;

}

int FLC::fuzzy_inference(int* inp, int nb_inp, int delta)
{
	int i, j;

	if (nb_inp!=this->n_inp) ev<<"ERROR: number of inputs differs from expected\n";

	Matrix** inp_f = new Matrix* [this->n_inp];

	for(i=0;i<n_inp;i++)
	{
		inp_f[i] = fuzzify(inp[i],delta); //delta = 4
		if (inp_f[i] == NULL) 
		{
			ev<<"Error fuzzifying crisp in\n";
			exit(1);
		}
	}

	Matrix minMat (this->bits_m, this->bits_n);
	Matrix maxMat (this->bits_m, this->bits_n);
	Vector maxVect (this->bits_m);
	Vector minMax (this->bits_m);


	Matrix* term = new Matrix(this->bits_m, this->bits_n);

	for(i=0;i<this->bits_m;i++)
		for(j=0;j<this->bits_n;j++)
			maxMat.set(i,j,0);

	for(i=0 ; i<this->n_rules ;i++)
	{
		for(j=0; j< this->bits_m;j++)
			minMax.set(j, 1);

		for(j=0; j<this->n_inp ; j++)
		{
			if (m_functions[j][this->rules[i][j]]->getTermRep(this->bits_m, this->bits_n, term)<0)
				ev<<"ERROR: error creating term\n";
			term->min_matrix(inp_f[j], &minMat);
			minMat.max_column(&maxVect);
			minMax.min_vect(&maxVect, &minMax);
			
			
		}
		m_functions[n_inp][rules[i][n_inp]]->getTermRep(bits_m, bits_n, term);
		term->min_vector(&minMax, &minMat);
		minMat.max_matrix(&maxMat, &maxMat);

	}

	//maxMat.print();

	delete term;

	for(i=0;i<n_inp;i++)
		delete inp_f[i];
	int result = defuzify(&maxMat);
	return result;
}


void FLC::handleMessage(cMessage *msg)
{
	if (!strcmp(msg->getName(),"start_flc"))
	{

	    ev << "Computing new W_HP" << endl;
	    int wantedDelayMP=1100;
	    int wantedDelayHP = 700;//(int)getParentModule()->par("delayLimit");
	    int currentDelayMP = round((double)getParentModule()->getSubmodule("sink")->par("meanDelayMP"));
	    int currentDelayHP=round((double)getParentModule()->getSubmodule("sink")->par("meanDelayHP"));
	    int W_HP = (int)getParentModule()->getSubmodule("scheduler")->par("W_HP");
	    int B = 30;//(int)getParentModule()->getSubmodule("netwrk")->par("B");
	    ev<<"FLC has mean HP delay of "<< currentDelayHP<<"ms"<<endl;
	    ev<<"FLC has mean MP delay of "<< currentDelayMP<<"ms"<<endl;
	    ev<<"W_HP is "<< W_HP<<endl;
	    int new_W_HP = W_HP;
		int diffHP= wantedDelayHP - currentDelayHP;
		int diffMP= wantedDelayMP - currentDelayMP;

		qtime.record (currentDelayHP);
		qtime.record (currentDelayMP);
		ev<<" Unscalled diff for HP = "<<diffHP<<"\n";
		ev<<" Unscalled diff for MP = "<<diffMP<<"\n";

        diffHP = scale(0, 62, -15000, 15000, diffHP);
        diffMP = scale(0, 62, -15000, 15000, diffMP);
		W_HP = scale(0, 62, 0, B, W_HP);
		ev<<" Scalled diff for HP = "<<diffHP<<"\n";
		ev<<" Scalled diff for MP = "<<diffMP<<"\n";
		int delta = 0;//(int) getParentModule()->par("delta");
		int inp[2]={diffHP,diffMP};
		
		int result;
		if (currentDelayHP > (wantedDelayHP + 100)) {
		            int inp[2] = { diffHP, 0 };
		            ev << "HP not stable. Ignoring MP in fuzzy inference." << endl;
		            result = fuzzy_inference(inp, 2, delta);
		        } else {
		            int inp[2] = { diffHP, diffMP };
		            ev << "HP stable. Using both HP and MP in fuzzy inference." << endl;
		            result = fuzzy_inference(inp, 2, delta);
		        }
		result_dep.record (result);
		ev<<"The fuzzy output is:"<<result<<"\n";

		int res = round(scale((B * -1)/2, B/2, 0, 62, result));
		ev<<" Scaled Result = "<<res<<"\n";

		res_dep.record (res);

		new_W_HP = new_W_HP + res;

		if (new_W_HP>B) new_W_HP = B-1;
		if (new_W_HP<1) new_W_HP = 1;


		cPar& W_HP_r = getParentModule()->getSubmodule("scheduler")->par("W_HP");
		W_HP_r.setIntValue(new_W_HP);

		ev<<"New Pound: "<<new_W_HP<<"\n\n";
		
		qtimew.record(new_W_HP);
		//cMessage *job = new cMessage("clear");
		//sendDirect(job, getParentModule()->getSubmodule("netwrk")->gate("in"));
		delete msg;
	}
}


