/*
 * A306408  a(n) = Sum_{d|n} (-1)^omega(n/d) * d.
 * https://oeis.org/A306408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^omega(n/d) * d
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Smallest‑prime‑factor sieve (dynamic, grows to cover the
   largest n that has been requested).  The array holds the smallest
   prime factor for each integer ≥2 and is stored as uint64_t*; the
   index type idx_t is also uint64_t, so the cast is safe in C and C++. */
static uint64_t *spf = NULL;     /* spf[x] = smallest prime factor of x (x ≥ 2) */
static idx_t    spf_limit = 0;   /* largest index for which spf is valid */

/* Expand the sieve to include at least n.  Complexity O(n log log n). */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;          /* already built */

    /* free old sieve if it exists */
    if (spf)
        free(spf);

    /* allocate new table */
    spf = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!spf)
        abort();         /* cannot continue without memory */

    /* init: every number is a potential prime */
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;

    /* classic sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j)
                    spf[j] = i;     /* record smallest prime factor */
            }
        }
    }
    spf_limit = n;
}

/* Compute a(n) according to the multiplicative definition:
   a(p^e) = p^e - (p^e - 1)/(p - 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;    /* undefined for 0; return 1 for safety */
    if (n == 1)
        return (seq_t)1;    /* a(1) = 1 */

    ensure_spf(n);          /* build spf table up to n if necessary */

    seq_t result = 1;       /* multiplicative identity */
    idx_t temp = n;

    while (temp > 1) {
        idx_t      p = spf[temp];   /* smallest prime factor of temp   */
        idx_t      e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* compute p^e as a seq_t */
        seq_t pe = 1;
        for (idx_t i = 0; i < e; ++i)
            pe *= (seq_t)p;

        /* factor a(p^e) = p^e - (p^e - 1)/(p - 1) */
        seq_t sum_pow = (pe - (seq_t)1) / (((seq_t)p) - (seq_t)1);
        seq_t coeff   = pe - sum_pow;

        result *= coeff;     /* combine multiplicative parts */
    }

    return result;
}
