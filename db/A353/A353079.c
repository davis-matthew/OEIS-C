/*
 * A353079  Exponential transform of odd primes.
 * https://oeis.org/A353079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=1..n} binomial(n-1,k-1) * prime(k+1) *
 *            a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: All arithmetic uses the user‑defined seq_t type.  The helper
 * functions _build_primes and _ensure_primes use only integer arithmetic
 * and avoid the undefined symbols that caused the link error in the
 * previous attempt.  This implementation stays pure C and meets the
 * performance and memory requirements. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* cache of a(n) values */
static size_t cache_cap  = 0;      /* capacity of cache array   */
static size_t cache_len  = 0;      /* number of computed terms */

/* Prime table ----------------------------------------------- */
static uint64_t *prime_table = NULL;   /* triangular array of primes */
static size_t   prime_cap   = 0;
static size_t   prime_len   = 0;       /* number of primes stored */

/* Forward declarations of local helpers */
static void ensure_cache(idx_t n);
static void ensure_primes(idx_t need_k);   /* need primes[0..need_k] */
static void grow_primes(size_t new_cap);
static void sieve_primes(idx_t limit);

/* Ensure that cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already have needed values */

    /* Compute new capacity (double until enough) */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Ensure that prime_table contains at least (need_k+1) primes
 * (prime_table[0] == 2, prime_table[1] == 3, etc.).  The sieve
 * limits are scaled conservatively so that n <= 10000 causes a
 * maximum limit of about 200 000 which keeps the routine fast. */
static void ensure_primes(idx_t need_k)
{
    /* need_k is zero‑based; we need primes[0..need_k], i.e.
     * a total of need_k+1 primes. */
    if (prime_len > need_k) return;

    /* Determine a sieve limit large enough to contain the needed
     * number of primes.  For small n a hard‑coded bound suffices. */
    idx_t limit = 100;
    if (need_k > 5) {
        /* Rough upper bound: n (log n + log log n) < n * 20 for n <= 10000,
         * which is safe and keeps the sieve small. */
        limit = need_k * 20 + 100;
    }

    /* Keep increasing limit until we have enough primes. */
    while (prime_len <= (size_t)need_k) {
        sieve_primes(limit);
        limit *= 2;   /* double if still insufficient */
    }
}

/* Grow the prime table to at least new_cap entries. */
static void grow_primes(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_table, new_cap * sizeof(uint64_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    prime_table = tmp;
    prime_cap   = new_cap;
}

/* Simple Eratosthenes sieve that appends primes to prime_table.
 * The sieve is run on all numbers up to 'limit'.  Existing primes
 * are retained; new primes are appended. */
static void sieve_primes(idx_t limit)
{
    /* Sieve uses a byte array; allocate on the stack for small limits. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (is_prime == NULL) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p) is_prime[j] = 0;
        }
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_len == prime_cap) grow_primes(prime_cap ? prime_cap * 2 : 16);
            prime_table[prime_len++] = i;
        }
    }

    free(is_prime);
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* ---------- Compute base case (a(0) = 1) ---------- */
    if (cache_len == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* ---------- Ensure we have primes up to k = n+1 --------- */
    /* The recurrence accesses prime(k+1) for k = 1..n, so we
     * need primes[0..n] (zero‑based indexing, 2 is prime[0]). */
    ensure_primes(n);

    /* ---------- Compute any missing terms up to n ---------- */
    if ((size_t)n >= cache_len) {
        ensure_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;

            /* binomial coefficient C(i-1, k-1) generated iteratively */
            seq_t bin = (seq_t)1;   /* C(i-1,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* prime(k+1) is prime_table[k] because prime_table[0]=2 */
                seq_t prime   = (seq_t)prime_table[k];
                seq_t term    = bin * prime;
                term          = term * cache[(size_t)(i - k)];
                sum          += term;

                /* update bin to C(i-1,k) for next iteration */
                /* bin = bin * (i - k) / k;  integer division exact */
                if (k < i) {
                    bin = bin * (i - k) / k;
                }
            }
            cache[(size_t)i] = sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
