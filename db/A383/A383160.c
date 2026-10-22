/*
 * A383160  a(n) is the numerator of the mean of the maximum exponents in the
 *          prime factorizations of the unitary divisors of n.
 * https://oeis.org/A383160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator( Σ_{d|n, gcd(d,n/d)=1} A051903(d) / 2^ω(n) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(n) is the number of distinct prime factors of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* safety for n==0 */

    /* factorization: store exponents of distinct prime factors */
    uint64_t exps[64];
    int k = 0;
    uint64_t m = (uint64_t)n;

    if (m % 2ULL == 0ULL) {
        int e = 0;
        while (m % 2ULL == 0ULL) { m /= 2ULL; ++e; }
        exps[k++] = e;
    }
    for (uint64_t p = 3ULL; p <= m / p; p += 2ULL) {
        if (m % p == 0ULL) {
            int e = 0;
            while (m % p == 0ULL) { m /= p; ++e; }
            exps[k++] = e;
        }
    }
    if (m > 1ULL) {           /* remaining prime factor */
        exps[k++] = 1;
    }

    /* sort exponents increasingly (k <= 16 for 64‑bit numbers) */
    for (int i = 1; i < k; ++i) {
        int cur = exps[i];
        int j = i - 1;
        while (j >= 0 && exps[j] > cur) {
            exps[j + 1] = exps[j];
            --j;
        }
        exps[j + 1] = cur;
    }

    /* compute S = Σ max exponent over all unitary divisors */
    seq_t S = (seq_t)0;
    for (int i = 0; i < k; ) {
        int j = i;
        while (j < k && exps[j] == exps[i]) ++j;
        int cnt_e = j - i;                     /* multiplicity of this exponent */
        seq_t mult = ((seq_t)1 << cnt_e) - (seq_t)1;   /* (2^cnt_e) - 1 */
        seq_t smaller = ((seq_t)1 << i);       /* 2^(number of smaller exponents) */
        S += (seq_t)exps[i] * mult * smaller;
        i = j;
    }

    if (S == (seq_t)0) return (seq_t)0;        /* a(1) case */

    /* reduce numerator by common powers of 2 */
    int v2 = 0;
    seq_t tmp = S;
    while ((tmp & (seq_t)1) == (seq_t)0) {
        tmp >>= 1;
        ++v2;
    }
    int reduce = v2 < k ? v2 : k;

    return S >> reduce;
}
