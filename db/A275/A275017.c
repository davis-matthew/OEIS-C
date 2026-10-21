/*
 * A275017  a(1)=1, a(2)=2, a(n) = prime(n-2) - a(n-2) for n > 2.
 * https://oeis.org/A275017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = prime(n-2) – a(n-2) (n > 2)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime storage (sequential sieve) ---------- */
static uint64_t *prime_tbl = NULL;   /* array of primes (uint64_t) */
static size_t   prime_cnt = 0;       /* number of primes stored   */
static size_t   prime_cap = 0;       /* allocated capacity       */
static uint64_t prime_next = 2;      /* next integer to test     */

/* add a prime to the table, growing the array if needed */
static void prime_add(uint64_t val)
{
    if (prime_cap == 0 || prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        if (!tmp) abort();            /* allocation failed – terminate */
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
    prime_tbl[prime_cnt++] = val;
}

/* test whether x is divisible by any stored prime ≤ sqrt(x) */
static bool is_not_prime(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p * p > x) break;
        if (x % p == 0) return true;
    }
    return false;
}

/* generate primes until we have at least need primes */
static void ensure_primes(size_t need)
{
    while (prime_cnt < need) {
        if (!is_not_prime(prime_next))
            prime_add(prime_next);
        ++prime_next;
    }
}

/* ---------- Sequence memoization ---------- */
static seq_t *seq_cache = NULL;  /* a(1) stored at index 0   */
static size_t seq_cap  = 0;      /* capacity of seq_cache     */
static size_t seq_len  = 0;      /* number of terms stored    */

/* grow seq_cache to at least needed capacity */
static void seq_ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failure */
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ---------- Main function ---------- */
seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 – return 0 */

    /* initialise cache with first two terms on first call */
    if (seq_len == 0) {
        seq_ensure_capacity(2);
        seq_cache[0] = (seq_t)1;   /* a(1) */
        seq_cache[1] = (seq_t)2;   /* a(2) */
        seq_len     = 2;
    }

    /* extend cache up to n if necessary */
    if ((idx_t)seq_len < n) {
        seq_ensure_capacity((size_t)n);          /* at least n terms */

        /* compute each new term using the recurrence */
        for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {
            if (i == 1) { seq_cache[0] = (seq_t)1; continue; }
            if (i == 2) { seq_cache[1] = (seq_t)2; continue; }

            /* need prime(i‑2) */
            ensure_primes((size_t)(i - 2));
            uint64_t p = prime_tbl[(size_t)(i - 3)];   /* 0‑based index */

            seq_t prev = seq_cache[(size_t)(i - 3)];   /* a(i‑2) */
            seq_cache[(size_t)(i - 1)] = (seq_t)p - prev;
        }
        seq_len = (size_t)n;
    }

    /* return requested term (a(1) is at index 0) */
    return seq_cache[(size_t)(n - 1)];
}
