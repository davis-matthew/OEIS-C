/*
 * A366531  Sum of even prime indices of n.
 * https://oeis.org/A366531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) – A366528(n); i.e. sum of the even prime
 *            indices of n. The prime index of a prime p is the 1‑based
 *            position of p in the list of all primes. Only prime indices that
 *            are even contribute – each is multiplied by the exponent of the
 *            corresponding prime factor in n. This implementatio...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime table handling ------------------------------------ */

static uint64_t *g_primes = NULL;        /* array of primes */
static size_t   g_prime_cnt = 0;         /* number of primes stored   */
static size_t   g_prime_limit = 0;       /* largest number sieved up to */

/* index_of[p] holds the 1‑based index of prime p (0 if p is not prime) */
static size_t *g_index_of = NULL;

/* Build a new prime table up to 'limit' (inclusive).  Overwrites any
   previously cached table. */
static void
build_primes_upto(size_t limit)
{
    /* free old tables */
    free(g_primes);
    free(g_index_of);

    g_primes     = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    g_index_of   = (size_t   *)calloc(limit + 1, sizeof(size_t));
    if (g_primes == NULL || g_index_of == NULL)
        exit(EXIT_FAILURE);              /* out of memory */

    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (is_comp == NULL)
        exit(EXIT_FAILURE);

    g_prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            g_primes[g_prime_cnt] = i;
            ++g_prime_cnt;
            g_index_of[i] = g_prime_cnt; /* 1‑based index */

            if (i * i <= limit)          /* mark composites */
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
        }
    }

    free(is_comp);
    g_prime_limit = limit;
}

/* Ensure that primes up to 'limit' have been sieved.  If a larger limit
   is requested, rebuild the prime tables. */
static void
ensure_primes_up_to(size_t limit)
{
    if (limit <= g_prime_limit)
        return;
    build_primes_upto(limit);
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* outside offset – any value OK */

    /* we must factor n, so we need primes up to at least n */
    ensure_primes_up_to((size_t)n);

    uint64_t m = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* trial division by the precomputed primes */
    for (size_t pi = 0; pi < g_prime_cnt && g_primes[pi] * g_primes[pi] <= m; ++pi) {
        uint64_t p = g_primes[pi];
        if (m % p != 0)
            continue;

        /* count exponent of p in m */
        size_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* add contribution only if the prime index is even */
        if ((g_index_of[(size_t)p] & 1) == 0)
            sum += (seq_t)exp * (seq_t)g_index_of[(size_t)p];
    }

    /* if anything left, it is a prime factor > sqrt(original n) */
    if (m > 1) {
        size_t idx = g_index_of[(size_t)m];   /* its prime index */
        if ((idx & 1) == 0)
            sum += (seq_t)idx;               /* exponent is 1 */
    }

    return sum;
}
