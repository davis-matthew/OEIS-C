/*
 * A330945  Numbers whose prime indices are not all prime numbers.
 * https://oeis.org/A330945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer whose prime indices are not all prime
 *            numbers (computed by sieve)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global data for the sieve and the sequence cache */
static idx_t          *spf          = NULL;   /* smallest prime factor for each number */
static idx_t           spf_limit    = 0;     /* maximum n for which spf is valid   */
static idx_t          *prime_to_index = NULL; /* prime value -> its 1‑based index   */
static bool           *index_is_prime = NULL; /* index_is_prime[i] is true iff i is prime (i>=1) */
static idx_t          *primes        = NULL;   /* list of primes */
static size_t         prime_count   = 0;
static size_t         prime_capacity= 0;

/* Simple primality test for integers (indices are small). */
static bool is_prime_int(size_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (size_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Build the sieve up to 'limit'.  All global tables are rebuilt. */
static void init_sieve(idx_t limit)
{
    /* free old tables if any */
    if (spf)        free(spf);
    if (prime_to_index) free(prime_to_index);
    if (index_is_prime) free(index_is_prime);
    if (primes)      free(primes);

    /* allocate new tables */
    spf           = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    prime_to_index= (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    index_is_prime= (bool*)  malloc((size_t)(limit + 1) * sizeof(bool));

    if (!spf || !prime_to_index || !index_is_prime) abort();

    for (idx_t i = 0; (idx_t)i <= limit; ++i)
    {
        spf[i] = 0;
        prime_to_index[i] = 0;
    }

    spf[0] = 0;
    spf[1] = 1;
    prime_capacity = 0;
    prime_count = 0;
    primes = NULL;

    /* Linear sieve (O(n)) */
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (prime_count >= prime_capacity)
            {
                size_t new_cap = prime_capacity ? prime_capacity * 2 : 128;
                primes = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
                if (!primes) abort();
                prime_capacity = new_cap;
            }
            primes[prime_count++] = i;
            if (prime_count < prime_capacity && i > limit / primes[0]) continue;
        }

        for (size_t j = 0; j < prime_count; ++j)
        {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > limit) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }

    /* Build prime_to_index and index_is_prime */
    for (size_t i = 0; i < prime_count; ++i)
    {
        prime_to_index[primes[i]] = (idx_t)(i + 1); /* 1‑based */
    }
    for (size_t i = 1; i <= prime_count; ++i)
    {
        index_is_prime[i] = is_prime_int(i);
    }

    /* Mark indices beyond prime_count as not prime */
    for (size_t i = prime_count + 1; i <= limit; ++i)
        index_is_prime[i] = false;

    spf_limit = limit;
}

/* Ensure the sieve covers at least 'limit' */
static void ensure_limit(idx_t limit)
{
    if (limit <= spf_limit) return;
    /* Increase limit by factor 2, but at least enough for 'limit' */
    idx_t new_limit = spf_limit ? spf_limit * 2 : 1000;
    if (new_limit < limit) new_limit = limit * 2;
    init_sieve(new_limit);
}

/* Return true if 'x' has a prime factor whose index is not prime (i.e., qualifies). */
static bool has_nonprime_index(idx_t x)
{
    while (x > 1)
    {
        idx_t p = spf[x];
        if (p == 0) p = x;                   /* safety, should not happen */
        idx_t idx = prime_to_index[p];
        if (idx <= 1 || !index_is_prime[idx])
            return true;
        while (x % p == 0)
            x /= p;
    }
    return false;
}

/* ------------------------------------------------------------ */
/* Sequence function                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 2;   /* numbers to test, starting from 2 */

    /* Initialise the sieve to a small limit on first call */
    if (spf_limit == 0)
        init_sieve(100000);   /* 1e5 is sufficient for n up to about 1e4 */

    /* Generate terms until we have the nth */
    while (cache_len < (size_t)n)
    {
        /* Extend sieve if candidate is outside current range */
        while (next_candidate > spf_limit)
        {
            idx_t new_lim = spf_limit ? spf_limit * 2 : 1000;
            if (new_lim < next_candidate) new_lim = next_candidate * 2;
            ensure_limit(new_lim);
        }

        if (has_nonprime_index(next_candidate))
        {
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!cache) abort();
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
