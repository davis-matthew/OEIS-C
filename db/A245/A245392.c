/*
 * A245392  Sum_{k, k|n} 2^(k-1) + Sum_{1<=k<=n, gcd(k,n)=1} 2^(k-1).
 * https://oeis.org/A245392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k|n} 2^(k-1) + sum_{1<=k<=n, gcd(k,n)=1} 2^(k-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"

/* Computes the nth term of OEIS sequence A245392. */
seq_t a(idx_t n)
{
    /* Sum over divisors of n */
    seq_t sum_div = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            sum_div += pow2(d - 1);            /* 2^(d-1) */
        }
    }

    /* Sum over 1 <= k <= n with gcd(k,n) == 1 */
    seq_t sum_coprime = 0;
    seq_t power = 1;          /* 2^0 */
    for (idx_t k = 1; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            sum_coprime += power;
        }
        power <<= 1;          /* next power of 2: 2^(k) */
    }

    return sum_div + sum_coprime;
}
