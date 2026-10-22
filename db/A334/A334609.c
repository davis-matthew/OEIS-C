/*
 * A334609  a(n) is the total number of down-steps after the final up-step in
 *          all 3_2-Dyck paths of length 4*n (n up-steps and 3*n down-steps).
 * https://oeis.org/A334609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(4*(n+1)+3 , n+1)/(4*(n+1)+3) * -
 *            9*binomial(4*n+3 , n)/(4*n+3) Derived equivalent form (avoids
 *            unnecessarily large binomials): a(n) = 3*binomial(4*n+6 ,
 *            n)/(n+1) - 9*binomial(4*n+2 , n-1)/n (for n >= 1; a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline __int128 binom128(idx_t a, idx_t b)
{
    /* Compute C(a,b) using 128‑bit arithmetic, safe for n such that
       the result fits in 64/128 bits.  For larger n this function is
       only used when seq_t is 128 or larger (the build that supports
       big integers). */
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    __int128 res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i);
        res /= i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* For seq_t types that are not larger than 128 bits we compute the
       binomial coefficients with the 128‑bit helper and cast the
       intermediate results to seq_t.  This guarantees no overflow for
       all n where the final value fits into seq_t (the tests for the
       uint64_t build stop before the values exceed 2^64).  For
       arbitrary‑precision builds the binomial() function is used
       directly, which may produce values larger than 128 bits. */
    if (sizeof(seq_t) <= sizeof(__int128)) {
        __int128 c1 = binom128(4 * n + 6, n);
        __int128 term1 = (c1 * 3) / (n + 1);

        __int128 c2 = binom128(4 * n + 2, n - 1);
        __int128 term2 = (c2 * 9) / n;

        return (seq_t)(term1 - term2);
    } else {
        seq_t c1 = binomial(4 * n + 6, n);
        seq_t term1 = (c1 * 3) / (n + 1);

        seq_t c2 = binomial(4 * n + 2, n - 1);
        seq_t term2 = (c2 * 9) / n;

        return term1 - term2;
    }
}
