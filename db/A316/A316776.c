/*
 * A316776  a(n) is the number of integers 0 < k < n such that n^2 - k^2 is a
 *          semiprime.
 * https://oeis.org/A316776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of integers 0 < k < n such that n^2 - k^2
 * is a semiprime (product of exactly two primes, counting multiplicity).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*-----------------------------------------------------------------
 *  Signatures required by the test harness
 *-----------------------------------------------------------------*/
seq_t a(idx_t n);

/*-----------------------------------------------------------------
 *  Static data: smallest‑prime‑factor table and its current size.
 *-----------------------------------------------------------------*/
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i */
static idx_t spf_limit = 0;        /* maximal i for which spf is valid */

/*-----------------------------------------------------------------
 *  Build or enlarge the spf table up to |limit| (inclusive).
 *-----------------------------------------------------------------*/
static void init_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;                       /* table already large enough */

    /* free old table */
    free(spf);

    /* allocate new table */
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) {                        /* allocation failed – no-op */
        spf_limit = 0;
        return;
    }

    /* initialise: number itself as prime factor (except 0,1) */
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (spf[i] == i)               /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j)
                    spf[j] = i;

    spf_limit = limit;
}

/*-----------------------------------------------------------------
 *  Count the total multiplicity of prime factors of |x|.
 *  (Essentially, big Omega function, but stops once it exceeds 2.)
 *-----------------------------------------------------------------*/
static size_t factor_count(idx_t x)
{
    size_t cnt = 0;
    while (x > 1 && cnt <= 2) {
        idx_t p = spf[x];
        x /= p;
        ++cnt;
    }
    return cnt;
}

/*-----------------------------------------------------------------
 *  Main sequence function.
 *-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* The factors n-k and n+k are at most 2n-1.  Compute SPF up to that. */
    idx_t limit = 2 * n - 1;
    init_spf(limit);

    seq_t result = 0;

    for (idx_t k = 1; k < n; ++k) {
        idx_t m1 = n - k;              /* n-k */
        idx_t m2 = n + k;              /* n+k */

        /* Total multiplicity of prime factors of (m1*m2). */
        size_t total = factor_count(m1) + factor_count(m2);
        if (total == 2)
            ++result;
    }

    return result;
}
