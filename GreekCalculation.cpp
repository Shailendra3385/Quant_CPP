#include <iostream>
#include <cmath>
#include <algorithm>

// Standard normal p r o b a b i l i t y d e n s i t y f unc t i on
double norm_pdf ( const double x ) {
	return ( 1.0 / ( pow(2*M_PI , 0.5 ) ) ) *exp (-0.5*x*x ) ;
}

double norm_cdf ( const double x ) {
	double k = 1.0 / ( 1.0 + 0.2316419*x ) ;
	double k_sum = k*(0.319381530 + k*(-0.356563782 + k *(1.781477937 + k
				*(-1.821255978 + 1.330274429*k ) ) ) ) ;
	if ( x >= 0.0 ) {
		return ( 1.0 - ( 1.0 / ( pow(2*M_PI , 0.5 ) ) ) *exp (-0.5*x*x ) * k_sum) ;
	} else {
		return 1.0 - norm_cdf (-x ) ;
	}
}

double d_j( const int j , const double S , const double K, const double r ,
			const double v , const double T) {
	return ( log(S/K) + ( r + (pow(-1 , j -1) ) *0.5* v*v ) *T) /( v *(pow(T, 0.5 ) ) ) ;
}

double call_price( const double& S , const double& K, const double& r ,
				const double& v , const double& T) {
	return S*norm_cdf( d_j ( 1 , S , K, r , v , T) )-K*exp(-r*T) *
			norm_cdf ( d_j ( 2 , S , K, r , v , T) ) ;
}

double call_delta( const double S , const double K, const double r , const
			double v , const double T) {
	return norm_cdf ( d_j( 1 , S , K, r , v , T) ) ;
}

double call_gamma ( const double S , const double K, const double r , const
				double v , const double T) {
	return norm_pdf ( d_j ( 1 , S , K, r , v , T) ) /(S*v*sqrt(T) ) ;
}

double call_vega( const double S , const double K, const double r , const
		double v , const double T) {
	return S*norm_pdf ( d_j ( 1 , S , K, r , v , T) ) *sqrt(T) ;
}

double call_theta( const double S , const double K, const double r , const
			double v , const double T) {
	return -(S*norm_pdf ( d_j ( 1 , S , K, r , v , T) ) *v ) /(2*sqrt(T) )
			- r *K*exp(-r*T) * norm_cdf( d_j ( 2 , S , K, r , v , T) ) ;
}

double call_rho( const double S , const double K, const double r , const
		double v , const double T) {
	return K*T*exp(-r*T) *norm_cdf( d_j ( 2 , S , K, r , v , T) ) ;
}

double put_price( const double S , const double K, const double r , const
			double v , const double T) {
	return -S*norm_cdf(-d_j( 1 , S , K, r , v , T) )+K*exp(-r*T)* norm_cdf(-d_j ( 2 ,
			S , K, r , v , T) ) ;
}

double put_delta( const double S , const double K, const double r , const
			double v , const double T) {
	return norm_cdf( d_j( 1 , S , K, r , v , T) ) - 1 ;
}

double put_gamma( const double S , const double K, const double r , const
			double v , const double T) {
	return call_gamma(S , K, r , v , T) ; 
}

double put_vega( const double S , const double K, const double r , const
			double v , const double T) {
	return call_vega(S , K, r , v , T) ; 
}

double put_theta( const double S , const double K, const double r , const
			double v , const double T) {
	return -(S*norm_pdf( d_j( 1 , S , K, r , v , T) )*v )/(2*sqrt(T) )
			+ r*K*exp(-r*T) *norm_cdf(-d_j( 2 , S , K, r , v , T) ) ;
}

double put_rho ( const double S , const double K, const double r , const double
			v , const double T) {
	return -T*K*exp(-r*T) * norm_cdf (-d_j ( 2 , S , K, r , v , T) ) ;
}

/*double gaussian_box_muller( ) {
	double x = 0.0 ;
	double y = 0.0 ;
	double euclid_sq = 0.0 ;

	do {
		x = 2.0 *rand()/static_cast<double>(RANDMAX) -1;
		y = 2.0 *rand()/static_cast<double>(RANDMAX) -1;
		euclid_sq = x*x + y*y ;
	} while ( euclid_sq >= 1.0 ) ;
	return x*sqrt(-2*log( euclid_sq ) / euclid_sq ) ;
}*/


int main ( int argc , char ** argv ) {
	// Fi r s t we c r e a t e the parameter l i s t
	double S = 100.0 ; // Opt ion p r i c e
	double K = 100.0 ; // S t r i k e p r i c e
	double r = 0.05 ; // Risk􀀀f r e e r a t e (5%)
	double v = 0.2 ; // Vo l a t i l i t y of the und e r l y ing (20%)
	double T = 1.0 ; // One year u n t i l e x p i r y
	// Then we c a l c u l a t e the c a l l / put v a l u e s and the Greeks
	double call = call_price (S , K, r , v , T) ;
	double call_delta_v = call_delta(S , K, r , v , T) ;
	double call_gamma_v = call_gamma (S , K, r , v , T) ;
	double call_vega_v = call_vega(S , K, r , v , T) ;
	double call_theta_v = call_theta (S , K, r , v , T) ;

	double call_rho_v = call_rho(S , K, r , v , T) ;
	double put = put_price(S , K, r , v , T) ;
	double put_delta_v = put_delta(S , K, r , v , T) ;
	double put_gamma_v = put_gamma(S , K, r , v , T) ;
	double put_vega_v = put_vega(S , K, r , v , T) ;
	double put_theta_v = put_theta(S , K, r , v , T) ;
	double put_rho_v = put_rho(S , K, r , v , T) ;

	std :: cout << "Underlying : " << S << std :: endl ;
	std :: cout << "Strike : " << K << std :: endl ;
	std :: cout << "Risk-Free Rate : " << r << std :: endl ;
	std :: cout << "Volatility : " << v << std :: endl ;
	std :: cout << "Maturity : " << T << std :: endl << std :: endl ;

	std :: cout << "Call Price : " << call << std :: endl ;
	std :: cout << "Call Delta : " << call_delta_v << std :: endl ;
	std :: cout << "Call Gamma: " << call_gamma_v << std :: endl ;
	std :: cout << "Call Vega : " << call_vega_v << std :: endl ;
	std :: cout << "Call Theta : " << call_theta_v << std :: endl ;
	std :: cout << "Call Rho : " << call_rho_v << std :: endl << std :: endl ;

	std :: cout << "Put Price : " << put << std :: endl ;
	std :: cout << "Put Delta : " << put_delta_v << std :: endl ;
	std :: cout << "Put Gamma: " << put_gamma_v << std :: endl ;
	std :: cout << "Put Vega : " << put_vega_v << std :: endl ;
	std :: cout << "Put Theta : " << put_theta_v << std :: endl ;
	std :: cout << "Put Rho : " << put_rho_v << std :: endl ;
	return 0 ;
}































