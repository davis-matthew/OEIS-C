/*
 * A362700  Expansion of e.g.f. 1/(1 + LambertW(-x * exp(x^2/2))).
 * https://oeis.org/A362700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..floor(n/2)} (n-2k)^(n-k) /* / (2^k * k! *
 *            (n-2k)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   factorial, pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */
#include "../utils/seq_pow.h"     /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/pow2.h"        /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* n! factor */
    seq_t fact_n = factorial(n);
    seq_t sum = 0;

    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t c = n - 2 * k;          /* n-2k  (>=0) */
        idx_t exp = n - k;            /* n-k   (>=0) */

        /* compute (n-2k)^(n-k)   careful with 0^0 */
        seq_t pow_c;
        if (c == 0 && exp == 0)
            pow_c = (seq_t)1;
        else
            pow_c = seq_pow((seq_t)c, exp);

        /* term = n! / ((n-2k)! * k! * 2^k) * pow_c */
        seq_t term = fact_n;
        term /= factorial(c);   /* divide by (n-2k)! */
        term /= factorial(k);   /* divide by k! */
        term /= pow2(k);        /* divide by 2^k */
        term *= pow_c;          /* multiply by (n-2k)^(n-k) */

        sum += term;
    }
    return sum;
}
