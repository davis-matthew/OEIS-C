/*
 * A279729  Sum of all the parts of the Goldbach partitions (p,q) of 2n such
 *          that all primes from p to q (inclusive) appear as a part in some
 *          Goldbach partition of p+q = 2n.
 * https://oeis.org/A279729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n * (number of Goldbach partitions (p,q) of 2n where p,q
 *            are odd primes, p <= q, and every prime in [p,q] occurs as a
 *            part of some Goldbach partition of 2n )
 * Range:     u8 n <= 29, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the code uses the prime tables found in ../utils/prime.h.
   It is efficient enough for n up to about 10000. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Prime table state */
static size_t g_max_limit = 0;           /* largest limit already sieved */
static uint8_t *g_is_prime = NULL;       /* g_is_prime[i] == 1 if i is prime */
static unsigned *g_prime_index = NULL;   /* index of prime i in prime list, (unsigned)-1 if not prime */
static int *g_prime_list = NULL;         /* list of primes in ascending order */
static size_t g_prime_count = 0;         /* number of primes in g_prime_list */

/* Allocate and sieve primes up to 'limit' (inclusive). */
static void init_primes(size_t limit)
{
    if (limit <= g_max_limit)
        return;                           /* already have enough */

    /* free any previous data */
    free(g_is_prime);
    free(g_prime_index);
    free(g_prime_list);

    /* allocate new tables, leaving room for 0..limit inclusive */
    g_is_prime = (uint8_t *)malloc(limit + 1);
    g_prime_index = (unsigned *)malloc((limit + 1) * sizeof(unsigned));
    if (!g_is_prime || !g_prime_index)   /* OOM – nothing we can do */
        return;

    for (size_t i = 0; i <= limit; ++i)
    {
        g_is_prime[i] = 1;
        g_prime_index[i] = (unsigned)-1;
    }
    g_is_prime[0] = g_is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
    {
        if (g_is_prime[i])
        {
            for (size_t j = i * i; j <= limit; j += i)
                g_is_prime[j] = 0;
        }
    }

    /* collect primes into list */
    g_prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (g_is_prime[i])
            ++g_prime_count;

    g_prime_list = (int *)malloc(g_prime_count * sizeof(int));
    if (!g_prime_list)
        return;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
    {
        if (g_is_prime[i])
        {
            g_prime_list[idx] = (int)i;
            g_prime_index[i] = (unsigned)idx;
            ++idx;
        }
    }

    g_max_limit = limit;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* 2n to be partitioned; it is even and >= 2. */
    idx_t m = n * 2;
    if (m < 4)                          /* no Goldbach partitions with odd primes */
        return 0;

    /* Ensure prime tables are ready up to m. */
    if ((size_t)m > g_max_limit)
        init_primes((size_t)m);

    /* Count primes <= m. */
    size_t primes_upto_m = 0;
    for (size_t i = 0; i < g_prime_count && g_prime_list[i] <= (int)m; ++i)
        ++primes_upto_m;

    /* The prefix array may have up to about 3000 entries for m <= 20000. */
    static size_t prefixBad[3000] = {0};

    /* Build prefixBad: number of “bad” primes (whose complement ≠ prime)
       among the first primes (starting from 2). */
    prefixBad[0] = 0;
    for (size_t i = 0; i < primes_upto_m; ++i)
    {
        int p = g_prime_list[i];
        int comp = (int)(m - p);
        /* 2 is ignored: it can never appear in a qualifying partition
           (all valid Goldbach partitions use odd primes). */
        unsigned bad = 0;
        if (p >= 3 && comp >= 3 && (size_t)comp <= (size_t)m && !g_is_prime[comp])
            bad = 1;
        prefixBad[i + 1] = prefixBad[i] + bad;
    }

    size_t count = 0;
    size_t half_m = m / 2;                /* upper bound for the first prime */
    /* Skip the prime 2: its index is 0. */
    size_t start = 0;
    if (g_prime_count > 0 && g_prime_list[0] == 2)
        start = 1;

    for (size_t i = start; i < primes_upto_m; ++i)
    {
        int p = g_prime_list[i];
        if ((size_t)p > half_m)           /* ensure p <= q */
            break;
        int q = (int)(m - p);
        if (q < 3 || !g_is_prime[q])      /* q must be an odd prime */
            continue;
        unsigned j = g_prime_index[q];    /* index of q in prime list */
        if (j >= primes_upto_m)
            continue;                     /* should not happen */
        /* No “bad” primes in the inclusive range [i..j] */
        if (prefixBad[j + 1] - prefixBad[i] == 0)
            ++count;
    }

    return (seq_t)m * (seq_t)count;
}
