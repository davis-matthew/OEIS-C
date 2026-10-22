/*
 * A365452  Largest number whose digits, in some base, sum to n and include no
 *          zeros.
 * https://oeis.org/A365452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = max_{1 ≤ i ≤ n} ( (i+1)^(n-i)*i + ((i+1)^(n-i)-1)/i )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   pow_by_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is indexed starting at 1.  The result is the largest value obtained by
   interpreting the digit string i,1,1,…,1 (n-i ones) in base i+1.  The
   exponentiation is performed by repeated squaring to keep the algorithm
   O(n log n).  No external headers beyond the standard ones are needed. */

#include <stdint.h>
#include "../utils/pow_by_square.h"

seq_t a(idx_t n)
{
    seq_t best = (seq_t)0;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t base   = (seq_t)(i + (idx_t)1);        /* base = i+1      */
        idx_t exp    = n - i;                       /* exponent k      */
        seq_t pow_val = pow_by_square(base, exp);   /* (i+1)^k          */

        /* value = i * pow_val + (pow_val - 1)/i  */
        seq_t val = pow_val * i + (pow_val - (seq_t)1) / i;

        if (val > best)
            best = val;
    }
    return best;
}
