/*
 * A375886  Numbers k such that 2k and 4k have the same number of Goldbach
 *          partitions into primes.
 * https://oeis.org/A375886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A375886 – Numbers k such that 2k and 4k have the same number of
    Goldbach partitions.
    The function builds the required primes on demand and caches all
    found k values. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----------  Cache for sequence values  ---------- */
static seq_t *seq_cache = NULL;   /* Cached a(n) values */
static size_t seq_len  = 0;       /* Number of cached values   */
static size_t seq_cap  = 0;       /* Current capacity          */

/* Append one value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ----------  Sieve of Eratosthenes  ---------- */
static uint8_t *sieve_flags = NULL;   /* 1 == prime, 0 == composite */
static size_t sieve_limit = 0;        /* Current upper bound of sieve */
static uint64_t *prime_list = NULL;   /* List of primes up to sieve_limit */
static size_t prime_count = 0;        /* Number of primes */

/* Construct a sieve up to (inclusive) 'limit'. */
static void build_sieve(size_t limit)
{
    if (limit < 2) limit = 2;

    uint8_t *prev_flags  = sieve_flags;
    uint64_t *prev_primes = prime_list;

    sieve_flags = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve_flags) abort();
    memset(sieve_flags, 1, limit + 1);
    sieve_flags[0] = sieve_flags[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve_flags[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve_flags[j] = 0;
        }
    }

    /* Count primes */
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_flags[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_flags[i]) prime_list[idx++] = (uint64_t)i;

    sieve_limit = limit;

    /* Free old memory */
    free(prev_flags);
    free(prev_primes);
}

/* Ensure that the sieve covers at least 'needed'.  If not, enlarge it. */
static void ensure_sieve(size_t needed)
{
    if (needed <= sieve_limit) return;
    size_t new_limit = sieve_limit ? sieve_limit : 2;
    while (new_limit < needed) new_limit <<= 1;
    build_sieve(new_limit);
}

/* ----------  Goldbach partition counter  ---------- */
/* Assumes N is even and <= sieve_limit. */
/* Counts unordered prime pairs p + q = N, with p <= q. */
static seq_t goldbach_count(uint64_t N)
{
    seq_t cnt = 0;
    uint64_t half = N >> 1;
    for (size_t i = 0; i < prime_count && prime_list[i] <= half; ++i) {
        uint64_t p = prime_list[i];
        if (sieve_flags[N - p]) ++cnt;
    }
    return cnt;
}

/* ----------  Sequence accessor  ---------- */
static uint64_t last_k = 1;   /* Next k to test (1‑based) */

/* External function: a(n) = the n‑th integer k with the property
   that Goldbach(2k) == Goldbach(4k).  The function is memoised and
   works for arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* Undefined for n=0 */
    size_t target = (size_t)n;        /* n fits into size_t for the test limits */

    /* Grow the cache until it contains first 'target' terms. */
    while (seq_len < target) {
        uint64_t k = last_k;
        uint64_t N4 = k << 2;         /* 4k */

        /* Ensure prime sieve covers up to 4k. */
        ensure_sieve((size_t)N4 + 1);

        seq_t c2 = goldbach_count(k << 1);  /* Goldbach(2k) */
        seq_t c4 = goldbach_count(N4);      /* Goldbach(4k) */

        if (c2 == c4) cache_append((seq_t)k);
        ++last_k;
    }

    return seq_cache[target - 1];
}
