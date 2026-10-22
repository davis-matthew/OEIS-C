/*
 * A358063  Expansion of e.g.f. exp( x * exp(-x^3) ).
 * https://oeis.org/A358063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} ((-n + 3k)^k)/(k! * (n - 3k)!)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"          /* pow_int(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    seq_t binom = 1;      /* binom(n, 0) */
    seq_t prod_fk = 1;    /* product_{i=1}^{3k} i / product_{i=1}^{k} i,  i.e. (3k)!/k! */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t base = (seq_t)(- (seq_t)n + (seq_t)(3 * k));
        seq_t power = pow_int(base, k);
        seq_t term = binom * prod_fk * power;
        sum += term;

        if (k == maxk)
            break;

        /* Update binom to binom(n, 3(k+1)) */
        idx_t m = k + 1;
        seq_t mult = (seq_t)(n - 3 * m + 1);
        mult *= (seq_t)(n - 3 * m + 2);
        mult *= (seq_t)(n - 3 * m + 3);

        seq_t divby = (seq_t)(3 * m);
        divby *= (seq_t)(3 * m - 1);
        divby *= (seq_t)(3 * m - 2);

        binom = binom * mult / divby;

        /* Update prod_fk to (3(k+1))!/ (k+1)! from k! */
        prod_fk = prod_fk * ( (seq_t)(3 * k + 1) *
                              (seq_t)(3 * k + 2) *
                              (seq_t)(3 * k + 3) ) / ( (seq_t)(k + 1) );
    }

    return sum;
}
