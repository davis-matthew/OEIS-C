/*
 * A345090  a(n) = Sum_{k=1..n} k^floor(1/gcd(k,2*n-k)).
 * https://oeis.org/A345090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^floor(1/gcd(k,2*n-k))
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That scalar equals Sum_{k=1..n} [gcd(k,2*n-k)==1 ? k : 1] */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1; behaviour for n==0 is undefined by OEIS spec */
    if (n == 0)
        return 0;

    seq_t sum   = 0;
    seq_t two_n = (seq_t)n * 2;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk   = (seq_t)k;          /* current k in seq_t form */
        seq_t rem  = two_n - kk;        /* 2*n - k */
        seq_t g    = gcd(kk, rem);      /* gcd(k, 2*n - k) */

        if (g == 1)
            sum += kk;                  /* k^1 = k */
        else
            sum += 1;                   /* k^0 = 1 */
    }
    return sum;
}
