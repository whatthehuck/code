
/*
 * return double number in (x,x+1) with uniform distribution
 */

db Uniform(db x=.0){
  return x+(db)(1.0+rand())/(RAND_MAX+2.0);
}

/*
 * return double number with Laplace distribution(miu, lambda)
 */

db Lap( db miu = .0, db lambda = 1.0) {
	db U = Uniform(- 0.5) ;
	return miu - lambda * sgn(U) * log( fabs(1.0 - 2* fabs(U)) ) ;
}
