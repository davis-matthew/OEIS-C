/*
 * A347614  a(n) = Sum_{k=1..n} n^Omega(k).
 * https://oeis.org/A347614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} n^{Omega(k)}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Omega(k) is the total number of prime factors of k (with multiplicity). */
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute the smallest prime factor (spf) array up to n */
static void build_spf(idx_t limit, uint64_t *spf)
{
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = i;
                }
            }
        }
    }
}

/* Helper: count Omega(k) using the spf array */
static idx_t omega_of(idx_t k, uint64_t *spf)
{
    idx_t cnt = 0;
    while (k > 1) {
        uint64_t p = spf[k];
        cnt++;
        k /= p;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate and build SPF array */
    uint64_t *spf = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!spf)
        return 0;                         /* allocation failure */
    build_spf(n, spf);

    /* Sum n^{Omega(k)} for k = 1 .. n */
    seq_t sum   = 0;
    seq_t base  = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t w = omega_of(k, spf);
        /* Compute base^w (w is small, at most log2(n)) */
        seq_t term = 1;
        for (idx_t i = 0; i < w; ++i)
            term *= base;
        sum += term;
    }

    free(spf);
    return sum;
}
