#include "convbond.h"
//' Convertible bond pricing.
//'
//' @param S_0 underlying initial price
//' @param sigma annualised implied volatility of the underlying
//' @param riskFreeInterestRate risk free rate
//' @param creditSpread spread to risk free rate
//' @param rateCompounding basis for discount rate. annual = 1, semi-annual = 2, etc. For the continious basis use rateCompounding = 0.
//' @param couponRate coupon on bond
//' @param couponFreq numer of coupons per year. Usually twice a year.
//' @param dividendProtectionRate divident protection threshold. In the uderlying model dividend protection implemented 
//' via adjustment to conversion rate (Dividend protection is almost always provided by an adjustment to the conversion price 
//' or conversion ratio).
//' @param dividendSchedule dividend schedule 2-column matrix. 1-column - time to dividend, 2-column - dividend rate. See demos in /demo.
//' @param conversionRatio number of shares per one convertible bond
//' @param noConversionPeriod restriction for conversion in years.
//' @param americanConversionType type of the conversion option. In almost all cases it is american. So we can convert any time. Set =0 if conversion possible only at maturity.
//' @param maturity maturity of the convertible
//' @param faceValue nominal amount of convertible bond
//' @param redemptionPremium premium at conversion. For example issuer can pay premium 10 percent of the faceValue at maturity to the holder (at redemtion).
//'  If so set parameter = 0.1. In almost all cases there is no redemption premium.
//' @param softCallStartTime time at which issuer will be able to start apply soft call option.
//' @param softCallBarrier level of equity price at which softCall can be applied. For example, 130 means: 
//' if price of equity will be greater 130, issuer can call CB at softCallStrike price
//' @param softCallStrike soft call option strike in percents of faceValue. Set to 0 if there is no option.
//' @param putStartTime time at which holder will be able to start apply put option.
//' @param putStrike  put option strike in percents of faceValue. Set to 0 if there is no option.
//' @param callStartTime time at which issuer will be able to start apply put option.
//' @param callStrike call option strike in percents of faceValue. Set to 0 if there is no option.
//' @param nbStepsPerYear number of steps in binomial computational grid per year)
//' @return price of convertible
//' @export
// [[Rcpp::export]]
double CBprice(double S_0,
	double sigma, 
	double riskFreeInterestRate,
	double creditSpread,
	double rateCompounding,
	double couponRate,
	double couponFreq,
	double dividendProtectionRate,
	NumericMatrix dividendSchedule,
	double conversionRatio,
	double noConversionPeriod,
	double americanConversionType,
	double maturity,
	double faceValue,
	double redemptionPremium = 0,
	double softCallStartTime = 0,
	double softCallBarrier = 0,
	double softCallStrike = 0,
	double putStartTime = 0,
	double putStrike = 0,
	double callStartTime = 0,
	double callStrike = 0,
	double nbStepsPerYear = 50)
{
	std::vector< std::vector< double > > rngDividendSchedule;
	NumericMatrixToVector2d(dividendSchedule, rngDividendSchedule);
	return CBpriceCpp(S_0, sigma, riskFreeInterestRate, creditSpread, rateCompounding, couponRate, couponFreq,
					dividendProtectionRate, rngDividendSchedule,conversionRatio,
					noConversionPeriod, americanConversionType, maturity, faceValue,
					redemptionPremium, softCallStartTime, softCallBarrier, softCallStrike,
					putStartTime, putStrike, callStartTime, callStrike,nbStepsPerYear);
	
}

//' #' @describeIn CBprice CBfindCoupon can be used for determining coupon at issuance of CB. This also can be done with 
//' CBpriceCouponWrapper and uniroot, but CBfindCoupon a little bit faster because of R interpreter overheads in uniroot. 
//' CBfindCoupon written in pure C++. Both methods use the same zeroin implementation of finding root of given function.
//' Initially this function was written for using in MS Excel spreadsheets wrapper of this package bacause of lack of fast 
//' optimizer in ms excel.
//' Please see examples proveded in /demo.
//' @export
// [[Rcpp::export]]
double CBfindCoupon(
	double S_0,
	double sigma, 
	double intRate0,
	double creditSpread,
	double rateCompounding,
	double couponFreq,
	double dividendProtectionRate,
	NumericMatrix RDividendSchedule,
	double conversionRatio,
	double noConversionPeriod,
	double americanConversionType,
	double maturity,
	double faceValue,
	double redemptionPremium = 0,
	double softCallStartTime = 0,
	double softCallBarrier = 0,
	double softCallStrike = 0,
	double putStartTime = 0,
	double putStrike = 0,
	double callStartTime = 0,
	double callStrike = 0,
	double nbStepsPerYear = 50,
  double targetBondPrice = 1.0
  )
{
	std::vector< std::vector< double > > rngDividendSchedule;
	NumericMatrixToVector2d(RDividendSchedule, rngDividendSchedule);
	return(
    convBondFindCouponCpp(S_0, sigma, intRate0, creditSpread, rateCompounding, couponFreq,
								dividendProtectionRate, rngDividendSchedule,conversionRatio,
								noConversionPeriod, americanConversionType, maturity, faceValue,
								redemptionPremium, softCallStartTime, softCallBarrier, softCallStrike,
								putStartTime, putStrike, callStartTime, callStrike, nbStepsPerYear, targetBondPrice)
  );
					
}
