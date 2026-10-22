/*
 * A397887  a(1) = 1; a(n) = n + a(A026238(n)) for n > 1.
 * https://oeis.org/A397887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n + a(A026238(n)), a(1) = 1 where A026238(n) = pi(n) if n
 *            is prime, = n – pi(n) – 1 if n is composite
 * Range:     u8 n <= 87, u16 n <= 9182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Global state ---------- */
static seq_t *cache      = NULL;   /* a(n) values, indexed by n-1   */
static size_t cache_cap  = 0;      /* allocated size of cache       */
static bool  *prime_tab  = NULL;   /* sieve table, true = prime     */
static seq_t *pi_tab      = NULL;   /* π(n) values                   */
static idx_t  prime_cap  = 0;       /* maximal n already sieved      */

/* ---------- Prime tables (sieve of Eratosthenes) ---------- */
static void
ensure_prime_tables(idx_t n)
{
    if (n <= prime_cap)
        return;                         /* already computed */

    size_t new_cap = (size_t)(n + 1);

    /* Resize the prime flag array */
    bool *new_prime = (bool*)realloc(prime_tab, new_cap);
    if (!new_prime) abort();

    /* Resize the π table */
    seq_t *new_pi = (seq_t*)realloc(pi_tab, new_cap * sizeof(seq_t));
    if (!new_pi) abort();

    /* Mark all numbers as prime initially */
    for (idx_t i = 0; i <= n; ++i)
        new_prime[i] = true;
    if (new_cap > 0) new_prime[0] = false;
    if (new_cap > 1) new_prime[1] = false;

    /* Sieving */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (new_prime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                new_prime[j] = false;
        }
    }

    /* Compute π(n) values */
    seq_t cnt = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (new_prime[i]) ++cnt;
        new_pi[i] = cnt;
    }

    prime_tab = new_prime;
    pi_tab    = new_pi;
    prime_cap = n;
}

/* ---------- Map A026238(n) ---------- */
static idx_t
a026238(idx_t n)
{
    /* n should be >= 2 when this function is used */
    ensure_prime_tables(n);
    if (prime_tab[n])
        return (idx_t)pi_tab[n];               /* prime case */
    else
        return n - (idx_t)pi_tab[n] - 1;       /* composite case */
}

/* ---------- Cache for a(n) ---------- */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    /* Initialize freshly allocated part to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache     = new_cache;
    cache_cap = new_cap;
}

/* ---------- Recursive computation with memoisation ---------- */
static seq_t
a_recur(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, covers any accidental n <= 0 */
        return 1;

    ensure_cache(n);

    seq_t cached_val = cache[n - 1];
    if (cached_val != 0)
        return cached_val;   /* already computed */

    idx_t next = a026238(n);
    seq_t val  = (seq_t)n + a_recur(next);
    cache[n - 1] = val;
    return val;
}

/* ---------- Public interface ---------- */
seq_t a(idx_t n)
{
    return a_recur(n);
}
