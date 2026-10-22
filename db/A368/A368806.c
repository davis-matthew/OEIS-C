/*
 * A368806  a(n) = Product_{i=1..j, j=1..k, k=1..n} i*j*k.
 * https://oeis.org/A368806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} k^(k*(k+1)/2) * k!^(k+1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 1, a(2) = 64, a(3) = 60466176, ... */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t ans   = 1;
    seq_t fact  = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        fact *= (seq_t)k;                                 // fact = k!
        seq_t exp1 = (seq_t)k * ((seq_t)k + 1) / 2;        // exponent for k^(...)
        seq_t term1 = big_pow((seq_t)k, exp1);            // k^(k(k+1)/2)
        seq_t term2 = big_pow(fact, (seq_t)k + 1);         // (k!)^(k+1)
        ans *= term1;
        ans *= term2;
    }
    return ans;
}
