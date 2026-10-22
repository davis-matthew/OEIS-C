/*
 * A322921  From Goldbach's conjecture: a(n) is the number of decompositions
 *          of 6n into a sum of two primes.
 * https://oeis.org/A322921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002375(3*n) – number of unordered prime pairs summing to
 *            6n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached results and prime sieve */
static seq_t  *cache      = NULL;  /* 1‑indexed, results */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  cur_max_n  = 0;      /* highest n computed so far */

static uint8_t *prime_bool = NULL;/* is_prime array */
static idx_t   sieve_limit = 0;   /* largest number sieved */

/* Build a prime table up to 'limit' (inclusive). Re‑allocates prime_bool. */
static void sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* discard old table and allocate new one */
    free(prime_bool);
    prime_bool = (uint8_t *)malloc(limit + 1);
    if (!prime_bool) exit(EXIT_FAILURE);

    /* mark all as prime initially */
    for (idx_t i = 0; i <= limit; ++i) prime_bool[i] = 1;
    prime_bool[0] = prime_bool[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (prime_bool[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_bool[j] = 0;
        }
    }
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n < 1 */

    /* Grow cache array as needed */
    if (cache_cap <= n)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n, memoising */
    for (idx_t k = cur_max_n + 1; k <= n; ++k)
    {
        idx_t target = 6 * k;
        sieve_up_to(target);

        seq_t count = 0;
        for (idx_t p = 2; p <= target / 2; ++p)
        {
            if (prime_bool[p] && prime_bool[target - p])
                count += (seq_t)1;
        }
        cache[k] = count;
    }
    cur_max_n = n;
    return cache[n];
}
