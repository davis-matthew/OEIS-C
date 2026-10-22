/*
 * A320674  Positive integers m with binary expansion (b_1, ..., b_k) (where k
 *          = A070939(m)) such that b_i = [m == 0 (mod prime(i))] for i = 1..k
 *          (where prime(i) denotes the i-th prime number and [] is an Iverson
 *          bracket).
 * https://oeis.org/A320674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A320674
   a(n) = the nth positive integer m whose binary expansion
   (b_1,…,b_k) satisfies b_i = [m ≡ 0 (mod prime(i))] for i = 1..k.
   Only even m can satisfy this. */
/* Implementation: linear search with caching; primes are pre‑computed
   once by a simple sieve.  The algorithm runs in O(n×k) time,
   where k is the bit‑length of the current candidate.  With n ≤ 10 000
   the running time is far below the given limits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------- helper: bit length of seq_t -------------------------- */
static size_t
bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x) { x >>= 1; len++; }
    return len;
}

/* -------------------------- prime sieve -------------------------- */
static uint64_t *primes = NULL;   /* array of primes, 1‑based indexing in code */
static size_t    prime_cnt = 0;   /* number of primes stored */

/* generate at least the first N_PRIMES primes (e.g. 20000) */
static void
init_primes(void)
{
    if (primes != NULL) return;                /* already done */

    /* limit chosen well above the maximal bit‑length needed for n ≤ 10000 */
    const size_t LIMIT = 200000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                     /* allocation failure → nothing */

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= LIMIT; m += p)
                is_prime[m] = false;

    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_cnt++;

    primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); primes = NULL; return; }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/* -------------------------- sequence with caching -------------------------- */
static seq_t *cache  = NULL; /* array of found terms */
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t  search_start = (seq_t)2; /* next even candidate */

/* Return the n-th term of the sequence (1‑based index). */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* index ≤0 not used */

    /* initialise everything on first call */
    static bool inited = false;
    if (!inited) {
        init_primes();
        cache = NULL;
        cache_size = 0;
        cache_cap  = 0;
        search_start = (seq_t)2;
        inited = true;
    }

    /* grow cache until it contains at least n terms */
    while (cache_size < (size_t)n) {
        seq_t m = search_start;
        search_start += (seq_t)2;          /* only even candidates */

        size_t k = bitlen_seq_t(m);       /* number of bits */
        bool ok = true;

        for (size_t i = 1; i <= k; ++i) {
            size_t shift = k - i;                 /* msb → lsb order */
            unsigned bit = (unsigned)((m >> shift) & (seq_t)1);
            uint64_t prime = primes[i - 1];        /* 0‑based array */

            bool divisible = ((m % (seq_t)prime) == (seq_t)0);
            if ((bit != 0) != divisible) {         /* mismatch */
                ok = false;
                break;
            }
        }

        if (ok) {
            /* expand cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failure */
                    free(cache);
                    cache = NULL;
                    cache_size = cache_cap = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = m;
        }
    }

    return cache[(size_t)(n - 1)];    /* cache is 0‑based */
}
