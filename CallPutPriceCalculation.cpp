
#include <iostream>
#include <cmath>
// Standard normal probability density function
double norm pdf ( const double& x ) {
	return ( 1.0 / ( pow(2*M_PI , 0.5 ) ) ) *exp (-0.5*x*x ) ;
}

// An approximat ion to the cumulat i v e d i s t r i b u t i o n f unc t i on
// f o r the s tandard normal d i s t r i b u t i o n
// Note : This i s a r e c u r s i v e f unc t i on
double norm_cdf( const double& x ) {
	double k = 1.0 / ( 1.0 + 0.2316419*x ) ;
	double k sum = k*(0.319381530 + k*(-0.356563782 + k*(1.781477937 +
					k*(-1.821255978 + 1.330274429*k ) ) ) ) ;
	if ( x >= 0.0 ) {
		return ( 1.0 -( 1.0 / ( pow(2*M_PI , 0.5 ) ) ) *exp (-0.5*x*x ) * k_sum) ;
	} else {
		return 1.0 - norm_cdf(-x ) ;
	}
}

double d_j( const int& j , const double& S , const double& K, const double& r ,
const double& v , const double& T) {
	return ( log(S/K) + ( r + (pow(-1 , j-1) ) *0.5*v*v ) *T) /( v*(pow(T, 0.5 ) ) ) ;
}

double call_price( const double& S , const double& K, const double& r ,
				const double& v , const double& T) {
	return S*norm_cdf( d_j ( 1 , S , K, r , v , T) )-K*exp(-r*T) *
			norm_cdf ( d_j ( 2 , S , K, r , v , T) ) ;
}


double put_price( const double& S , const double& K, const double& r ,
			const double& v , const double& T) {
return -S*norm_cdf(-d_j ( 1 , S , K, r , v , T) )+K*exp(-r*T) *
		norm_cdf(-d_j ( 2 , S , K, r , v , T) ) ;
}

int main ( int argc , char ** argv ) {
	// Fi r s t we c r e a t e the parameter l i s t
	double S = 100.0 ; // Opt ion p r i c e
	double K = 100.0 ; // S t r i k e p r i c e
	double r = 0.05 ; // Risk􀀀f r e e r a t e (5%)
	double v = 0.2 ; // Vo l a t i l i t y of the und e r l y ing (20%)
	double T = 1.0 ; // One year u n t i l e x p i r y
	// Then we c a l c u l a t e the c a l l / put v a l u e s
	double call = call_price(S , K, r , v , T) ;
	double put = put_price(S , K, r , v , T) ;
	// Fi n a l l y we output the parameters and p r i c e s
	std :: cout << "Under lying : " << S << std :: endl ;
	std :: cout << " Strike : " << K << std :: endl ;
	std :: cout << "Risk-Free Rate : " << r << std :: endl ;
	std :: cout << " Volatility : " << v << std :: endl ;
	std :: cout << "Maturity : " << T << std :: endl ;
	std :: cout << "Call Price : " << call << std :: endl ;
	std :: cout << "Put Price : " << put << std :: endl ;
	return 0 ;
}

