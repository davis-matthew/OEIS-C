/*
 * A401014  Maximum number of distinct prime divisors of n+k greater than k,
 *          over all integers k >= 1.
 * https://oeis.org/A401014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = max_{k≥1} #{p prime | p divides n+k and p > k}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine computes the OEIS sequence A401014.
   It uses a sieve for smallest prime factors up to 2·n, and for each
   k = 1..n it factors n+k counting only primes larger than k. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Smallest‑prime‑factor table (static to avoid rebuilding). */
static uint32_t *spf_table = NULL;
static idx_t   spf_limit = 0;

/* Ensure spf_table holds smallest prime factors up to 'needed'. */
static void ensure_spf(idx_t needed)
{
    if (needed <= spf_limit)
        return;
    /* Allocate a new table larger than required. */
    size_t new_size = (size_t)(needed + 1);
    uint32_t *new_spf = (uint32_t *)realloc(spf_table,
                                            new_size * sizeof(uint32_t));
    if (!new_spf)          /* allocation failed – abort */
        exit(1);
    spf_table = new_spf;
    spf_limit = needed;

    /* Initialise to 0 for clean sieve. */
    for (idx_t i = 0; i <= spf_limit; ++i)
        spf_table[i] = 0;

    /* Sieve to fill smallest prime factors. */
    for (idx_t p = 2; p <= spf_limit / p; ++p) {
        if (spf_table[p] == 0) {          /* p is prime */
            spf_table[p] = (uint32_t)p;
            for (idx_t q = p * p; q <= spf_limit; q += p)
                if (spf_table[q] == 0)
                    spf_table[q] = (uint32_t)p;
        }
    }
    /* Remaining numbers are primes themselves. */
    for (idx_t i = 2; i <= spf_limit; ++i)
        if (spf_table[i] == 0)
            spf_table[i] = (uint32_t)i;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for offset < 1 */

    /* Build sieve up to 2·n (the largest number we factor). */
    idx_t limit = n + n;
    ensure_spf(limit);

    seq_t best = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = n + k;
        seq_t cnt = (seq_t)0;

        /* Factor m using the spf_table. */
        idx_t tmp = m;
        while (tmp > 1) {
            uint32_t p = spf_table[tmp];
            if ((idx_t)p > k)
                ++cnt;
            while (tmp % p == 0)
                tmp /= p;
        }
        if (cnt > best)
            best = cnt;
    }
    return best;
}
