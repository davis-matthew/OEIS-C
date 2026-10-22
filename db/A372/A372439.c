/*
 * A372439  Numbers k such that the least binary index of k plus the least
 *          prime index of k is odd.
 * https://oeis.org/A372439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k whose least binary index plus the
   least prime index is odd.  The implementation uses a sieve for the
   smallest prime factor and checks the parity condition. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cached sequence values      */
static idx_t seq_cache_len = 0;   /* number of cached values     */
static idx_t last_checked   = 1;  /* last candidate examined     */

static uint64_t *spf = NULL;      /* smallest prime factors      */
static size_t    spf_limit = 0;   /* sieve coverage              */

static uint64_t *primes = NULL;   /* list of primes              */
static size_t    primes_cnt = 0;  /* number of primes in list    */

/* Ensure the sieve covers at least [0..limit].  The arrays are rebuilt
   from scratch each time.  This keeps the code simple and suitable for
   both integer and big builds. */
static void ensure_sieve(size_t limit)
{
    if (limit <= spf_limit) return;

    uint64_t *new_spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i <= limit / i) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint64_t)i;
                }
            }
        }
    }

    free(spf);
    spf = new_spf;

    free(primes);
    primes_cnt = 0;
    size_t prime_cap = limit / 10 + 16;
    primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {
            if (primes_cnt >= prime_cap) {
                prime_cap *= 2;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = i;
        }
    }

    spf_limit = limit;
}

/* Binary search for 1‑based index of prime p in the primes array. */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (int)(lo + 1);
}

/* Count trailing zero bits of an unsigned 64‑bit integer.  Assumes x>0. */
static int trailing_zeros_uint64(uint64_t x)
{
    int cnt = 0;
    while ((x & 1ULL) == 0ULL) {
        ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* Extend the cache to size at least n.  The cache is grown with
   allocations suitable for plain data (C) or for arbitrary‑precision
   types (C++). */
static void ensure_cache(idx_t n)
{
    if (seq_cache_len >= n) return;

    size_t new_len = (size_t)n;

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_len];
    for (idx_t i = 0; i < seq_cache_len; ++i) new_cache[i] = seq_cache[i];
    delete [] seq_cache;
    seq_cache = new_cache;
#else
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    seq_cache = new_cache;
#endif

    for (idx_t cand = last_checked + 1; ; ++cand) {
        if ((size_t)cand > spf_limit) ensure_sieve((size_t)cand);

        uint64_t k = (uint64_t)cand;
        int e = trailing_zeros_uint64(k) + 1;       /* least binary index   */
        uint64_t spf_val = spf[k];
        int pi;
        if (spf_val == 2) pi = 1;
        else pi = prime_index(spf_val);

        if ((e + pi) & 1) {                      /* parity test          */
            seq_cache[seq_cache_len++] = (seq_t)k;
            if (seq_cache_len == new_len) {
                last_checked = cand;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    ensure_cache(n);
    return seq_cache[n - 1];
}
