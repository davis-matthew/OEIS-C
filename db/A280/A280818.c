/*
 * A280818  a(0)=1; for n > 0, if 4n+1 is prime, then a(n)=4n+1, otherwise
 *          a(n)=(4n+1)/LPF(4n+1).
 * https://oeis.org/A280818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=4n+1 if 4n+1 is prime, otherwise
 *            a(n)=(4n+1)/LPF(4n+1), where LPF is the least prime factor
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    /* Special‐case the first term. */
    if (n == 0) return 1;

    /* 4*n+1 fits in idx_t (uint64_t) for the range of n we are interested in. */
    idx_t m64 = 4ULL * n + 1ULL;

    /* Find the least prime factor of m64. */
    idx_t l = lpf(m64);

    if (l == m64)           /* m64 is prime */
        return (seq_t)m64;
    else                    /* composite: divide by its least prime factor */
        return (seq_t)m64 / (seq_t)l;
}
