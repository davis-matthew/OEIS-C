/*
 * A338810  a(n) = (n!/2) * Sum_{k=1..n-1} d(k)*d(n-k)/(k*(n-k)), where d(n)
 *          is the number of divisors of n.
 * https://oeis.org/A338810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=1}^{n-1} d(k)*d(n-k)/k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   divisor_count_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* uint64_t divisor_count_u64(uint64_t n); */
#include "../utils/factorial.h"          /* seq_t factorial(idx_t n); */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                /* a(1) = 0 */

    seq_t fact = factorial(n - 1u);      /* (n‑1)! */
    seq_t sum  = (seq_t)0;

    for (idx_t k = (idx_t)1; k < n; ++k)
    {
        seq_t dk   = (seq_t)divisor_count_u64((uint64_t)k);
        seq_t dnk  = (seq_t)divisor_count_u64((uint64_t)(n - k));

        unsigned __int128 prod = (unsigned __int128)fact;
        prod *= (unsigned __int128)dk;
        prod *= (unsigned __int128)dnk;
        prod /= (unsigned __int128)k;            /* Integer division */

        sum += (seq_t)prod;
    }

    return sum;
}
