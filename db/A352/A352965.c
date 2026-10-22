/*
 * A352965  A variant of Van Eck's sequence where we only consider prime
 *          numbers: for n >= 0, if a(n) = a(n-p) for some prime number p,
 *          take the least such p and set a(n+1) = p; otherwise a(n+1) = 0.
 *          Start with a(1) = 0.
 * https://oeis.org/A352965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0. For n >= 1, let p be the least prime such that a(n) =
 *            a(n-p). If such a prime exists then a(n+1) = p, otherwise a(n+1)
 *            = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies required by the judge */
#include <stddef.h>   /* size_t, SIZE_MAX */
#include <stdlib.h>   /* malloc, realloc, free, calloc, exit */
#include <stdbool.h>  /* bool, true, false */

/* ------------------------------------------------------------------- */
/*  State for memoisation of a(n)                                        */
/* ------------------------------------------------------------------- */
static seq_t *cache = NULL;       /* 1‑based cache of a(n) values */
static size_t cache_cap = 0;      /* allocated capacity (elements)      */
static size_t cache_len = 0;      /* largest index computed so far      */

/* ------------------------------------------------------------------- */
/*  State for the list of primes up to the current limit               */
/* ------------------------------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes                   */
static size_t   primes_cnt = 0;   /* number of primes stored           */
static uint64_t primes_limit = 0; /* largest prime generated           */

/* ------------------------------------------------------------------- */
/*  Ensure that cache can hold indices up to n (inclusive).  Cache[0]
 *  is unused.  The function also guarantees that a(1)=0 is stored.
 * ------------------------------------------------------------------- */
static void
ensure_cache_capacity(idx_t n)
{
    if (n <= 1) {                /* we only need indices >= 1 */
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
            cache_cap = 1;
            cache[1] = 0;         /* a(1) = 0 by definition */
            cache_len = 1;
        }
        return;
    }

    if (n + 1 > SIZE_MAX) {
        /* Prevent overflow: the maximum index we will ever request is
           comfortably below SIZE_MAX, but this guard keeps the code
           well‑defined for pathological inputs. */
        exit(EXIT_FAILURE);
    }

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_len == 0) {
        /* First entry never written before: initialise a(1) */
        cache[1] = 0;
        cache_len = 1;
    }
}

/* ------------------------------------------------------------------- */
/*  Simple Sieve of Eratosthenes: generate all primes <= limit.  The
 *  primes array is rebuilt completely every time the limit is raised.
 * ------------------------------------------------------------------- */
static void
generate_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;          /* already enough */

    /* The sieve needs a size_t array because the size_t is used in
       pointer arithmetic for the boolean flags. */
    size_t L = (size_t)limit;
    bool *is_prime = (bool *)calloc(L + 1, sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);

    if (L >= 2) {
        for (size_t i = 2; i <= L; ++i) is_prime[i] = true;
    }

    for (size_t i = 2; i * i <= L; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= L; j += i) is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= L; ++i) if (is_prime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) exit(EXIT_FAILURE);

    size_t k = 0;
    for (size_t i = 2; i <= L; ++i) if (is_prime[i]) new_primes[k++] = i;

    free(is_prime);
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* ------------------------------------------------------------------- */
/*  The required OEIS function                                             */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1; for other values we
       return 0 as a safe default. */
    if (n <= 1) return 0;

    ensure_cache_capacity(n);

    /* Return the cached value if it is already computed. */
    if ((size_t)n <= cache_len) {
        return cache[(size_t)n];
    }

    /* Generate terms up to the requested index. */
    while (cache_len < (size_t)n) {
        size_t curr_index = cache_len;                   /* index of a(curr) */
        seq_t curr_val = cache[curr_index];

        seq_t next_val = 0;  /* default if no suitable prime found */

        /* We need primes up to the current index to check all possible p. */
        generate_primes_upto(curr_index);

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (curr_index <= p) continue;                     /* n-p would be < 1 */

            if (curr_val == cache[curr_index - p]) {            /* match */
                next_val = (seq_t)p;   /* conversion through constructor */
                break;                  /* least such p found */
            }
        }

        ++cache_len;
        cache[cache_len] = next_val;
    }

    return cache[(size_t)n];
}
