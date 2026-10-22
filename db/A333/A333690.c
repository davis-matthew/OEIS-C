/*
 * A333690  a(n) = n*Product_{k=1..n} floor(k^((n-k)/(n-k+1))).
 * https://oeis.org/A333690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Product_{k=1..n}floor(k^((n-k)/(n-k+1)))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   floor_k_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/floor_k_power.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0, but safe */

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = floor_k_power(k, n - k);
        prod *= term;
    }
    prod *= (seq_t)n;
    return prod;
}
