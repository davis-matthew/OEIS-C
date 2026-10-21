/*
 * A263837  Non-abundant numbers (or nonabundant numbers): complement of
 *          A005101; numbers k for which sigma(k) <= 2*k.
 * https://oeis.org/A263837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A263837: the non‑abundant numbers.
   A term n is included iff σ(n) ≤ 2·n, i.e. the sum of proper divisors
   of n is ≤ n.  The function builds a sieve that grows as needed and
   caches the sequence for quick repeated look‑ups. */

#include <stdlib.h>

static seq_t *nonab = NULL;        /* cache of non‑abundant numbers */
static size_t nonab_cnt = 0;       /* how many terms stored */
static size_t nonab_cap = 0;       /* cache capacity */

static seq_t *sigma = NULL;        /* sums of proper divisors */
static size_t sigma_cap = 0;       /* capacity of sigma array */

static idx_t max_limit = 0;        /* largest number has been sieved */

/* Expand the sieve to cover at least 'target' and update the cache. */
static void ensure_limit(idx_t target)
{
    if (max_limit >= target)
        return;

    /* Pick a new limit that is comfortably larger. */
    if (max_limit == 0)
        max_limit = target * 3 + 10;          /* first pass */
    else
    {
        /* grow by at least 50 % each time until we reach the target */
        while (max_limit < target)
            max_limit = max_limit + max_limit / 2 + 1;
    }

    /* Allocate sigma array if necessary */
    if (sigma_cap < (size_t)max_limit + 1)
    {
        seq_t *tmp = (seq_t *)realloc(sigma, (max_limit + 1) * sizeof(seq_t));
        sigma = tmp;
        sigma_cap = max_limit + 1;
    }

    /* Reset the sigma array */
    for (size_t i = 0; i < sigma_cap; ++i)
        sigma[i] = 0;

    /* Compute sums of proper divisors */
    for (idx_t i = 1; i <= max_limit / 2; ++i)
    {
        seq_t val = i;
        for (idx_t j = i * 2; j <= max_limit; j += i)
            sigma[j] += val;
    }

    /* Extend the non‑abundant list */
    idx_t start = (nonab_cnt == 0) ? 1 : (idx_t)nonab[nonab_cnt - 1] + 1;
    for (idx_t n = start; n <= max_limit; ++n)
    {
        seq_t sum = sigma[n];                    /* sum of proper divisors of n */
        if (sum <= (seq_t)n)                     /* i.e. sigma(n) ≤ 2·n */
        {
            if (nonab_cnt >= nonab_cap)
            {
                size_t new_cap = nonab_cap == 0 ? 256 : nonab_cap * 2;
                seq_t *tmp = (seq_t *)realloc(nonab, new_cap * sizeof(seq_t));
                nonab = tmp;
                nonab_cap = new_cap;
            }
            nonab[nonab_cnt++] = n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined, but safe */
    while (nonab_cnt < (size_t)n)
        ensure_limit((idx_t)(n * 3 + 10));
    return nonab[n - 1];
}
