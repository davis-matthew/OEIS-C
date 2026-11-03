/*
 * A294145  Sum of the smaller parts of the partitions of n into two parts
 *          with larger part squarefree.
 * https://oeis.org/A294145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i * mu(n-i)^2
 * Range:     u8 n <= 55, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n >> 1;          /* floor(n/2) */
    for (idx_t i = 1; i <= limit; ++i)
    {
        if (mu_of(n - i) != 0)    /* mu^2 = 1 iff n-i is squarefree */
            sum += (seq_t)i;
    }
    return sum;
}
