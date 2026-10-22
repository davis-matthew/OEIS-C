/*
 * A325891  a(1) = 1; a(n) = -Sum_{d|n, d<n} prime(n/d) * a(d).
 * https://oeis.org/A325891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A325891 (OEIS)  
 *  a(1) = 1;  a(n) = -∑_{d | n, d<n} prime(n/d)·a(d)
 *  (prime(k) = the k‑th prime number)
 *
 *  The function caches computed values and lazily builds a prime sieve up
 *  to the largest needed index.  It works for any integer type seq_t
 *  (signed) and idx_t = uint64_t.  It is compatible with C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (simple, on‑demand) ---------- */
static uint32_t *prime_list = NULL;   /* array of primes  (1‑based) */
static size_t   prime_cnt   = 0;      /* number of primes stored */

static void rebuild_primes(size_t limit)
{
    /* free previous data */
    free(prime_list);
    prime_cnt = 0;

    /* simple sieve of Eratosthenes */
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;
    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    /* store primes */
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_list[pos++] = (uint32_t)i;

    prime_cnt = cnt;
    free(sieve);
}

static void ensure_primes_needed(size_t need)
{
    if (prime_cnt >= need) return;
    /* Rough upper bound for the n‑th prime: n(ln n + ln ln n) < n·20 for n≤10⁴ */
    size_t bound = need * 20 + 100;
    if (bound < 100) bound = 100;
    rebuild_primes(bound);
}

/* ---------- Sequence cache ---------- */
static seq_t *cache   = NULL;      /* cache[1..computed_max] */
static size_t cache_cap = 0;       /* allocated size (>= computed_max+1) */
static size_t computed_max = 0;    /* largest n computed so far */

/* Helper to grow the cache */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 2;         /* small guard space */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                      /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n<1 – defensive return */

    /* first‑time initialization */
    if (!cache) {
        cache_cap = 3;                                    /* 0,1,2 indices at least */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;                             /* allocation failure */
        cache[1] = 1;                                     /* a(1) = 1 */
        computed_max = 1;
        ensure_primes_needed(2);                          /* we will need prime(2) soon */
    }

    ensure_cache(n);

    /* compute any missing values up to n */
    for (size_t m = computed_max + 1; m <= (size_t)n; ++m) {
        seq_t sum = 0;
        /* iterate over proper divisors d of m */
        for (idx_t i = 1; i * i <= (idx_t)m; ++i) {
            if ((idx_t)m % i == 0) {
                idx_t d1 = i;
                if (d1 < (idx_t)m) {           /* proper divisor check */
                    idx_t q = (idx_t)m / d1;   /* q ≥ 2 */
                    ensure_primes_needed((size_t)q);
                    seq_t prime_val = (seq_t)prime_list[(size_t)q - 1];
                    sum += prime_val * cache[d1];
                }
                idx_t d2 = (idx_t)m / i;
                if (d2 != d1 && d2 < (idx_t)m) {
                    idx_t q = (idx_t)m / d2;
                    ensure_primes_needed((size_t)q);
                    seq_t prime_val = (seq_t)prime_list[(size_t)q - 1];
                    sum += prime_val * cache[d2];
                }
            }
        }
        cache[m] = -sum;
    }
    computed_max = (size_t)n > computed_max ? (size_t)n : computed_max;
    return cache[(size_t)n];
}
