/*
 * A391226  Numbers k > 1 such that the least prime index of k plus the
 *          greatest prime index of k is even.
 * https://oeis.org/A391226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A391226:
   the sequence of integers k>1 such that the least prime index
   of k plus the greatest prime index of k is even.
   The implementation precomputes smallest prime factors up
   to a fixed limit and scans integers until the required
   term is reached. */
#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf          = NULL;   /* smallest prime factor for each number */
static uint32_t *prime_index  = NULL;   /* 1‑based index of each prime */
static const idx_t SIEVE_MAX  = 2000000;/* sieve upper bound – large enough for n≤10000 */

static void init_sieve(void)
{
    if (spf)
        return;                      /* already initialized */

    /* allocate storage */
    spf = (uint32_t *)malloc((size_t)(SIEVE_MAX + 1) * sizeof(uint32_t));
    prime_index = (uint32_t *)calloc((size_t)(SIEVE_MAX + 1), sizeof(uint32_t));
    if (!spf || !prime_index)
        abort();                      /* unlikely but safe */

    /* initialise spf[i] = i */
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
        spf[i] = (uint32_t)i;

    /* standard Sieve of Eratosthenes to fill spf */
    for (idx_t i = 2; i * i <= SIEVE_MAX; ++i)
    {
        if (spf[i] == i) /* i is prime */
        {
            for (idx_t j = i * i; j <= SIEVE_MAX; j += i)
                if (spf[j] == j)
                    spf[j] = (uint32_t)i;
        }
    }

    /* build prime index mapping.  The index is 1‑based. */
    uint32_t idx = 0;
    for (idx_t i = 2; i <= SIEVE_MAX; ++i)
    {
        if (spf[i] == i)          /* prime */
        {
            ++idx;
            prime_index[i] = idx;
        }
    }
}

/*  Compute the nth term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    init_sieve();

    idx_t candidate = 2;            /* first integer >1 */
    idx_t found = 0;                /* number of qualifying integers seen */

    while (found < n)
    {
        /* factorise candidate using the precomputed spf array */
        idx_t x = candidate;
        idx_t min_idx = SIEVE_MAX + 1;  /* something larger than any real idx */
        idx_t max_idx = 0;

        while (x > 1)
        {
            uint32_t p = spf[x];   /* smallest prime factor of x */
            idx_t p_idx = prime_index[p];

            if (p_idx < min_idx) min_idx = p_idx;
            if (p_idx > max_idx) max_idx = p_idx;

            /* remove all powers of p from x */
            while (x % p == 0)
                x /= p;
        }

        /* test the parity condition */
        if (((min_idx + max_idx) & 1) == 0)
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }

        ++candidate;
    }

    /* should never reach here */
    return (seq_t)0;
}
