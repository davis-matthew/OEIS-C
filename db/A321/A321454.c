/*
 * A321454  Numbers that can be factored into two or more factors all having
 *          the same sum of prime indices.
 * https://oeis.org/A321454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   backtrack_partition, build_primes_upto, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321454 – Numbers that can be factored into two or more factors all having the same sum of prime indices.
   The code maintains a cache of sequence values. For each new integer it
   factorises it using a sieve of primes up to SIEVE_LIMIT, builds the multiset
   of prime indices of the factors, and checks whether that multiset can be
   partitioned into ≥2 blocks with equal sum by a backtracking routine. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"
#include "../utils/backtrack_partition.h"

#define SIEVE_LIMIT 2000000      /* primes up to this limit are precomputed */

/* static data --------------------------------------------------------- */

static uint64_t *primes      = NULL;   /* list of primes up to SIEVE_LIMIT */
static size_t    prime_count = 0;      /* number of primes found */

static uint32_t *prime_index = NULL;   /* prime_index[p] = 1‑based index of prime p (p <= SIEVE_LIMIT) */

/* initialise prime tables ------------------------------------------- */

static void init_primes(void)
{
    /* build list of primes ------------------------------------------------ */
    build_primes_upto(SIEVE_LIMIT, &primes, &prime_count);

    /* prime_index array --------------------------------------------------- */
    prime_index = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!prime_index) abort();
    memset(prime_index, 0, (SIEVE_LIMIT + 1) * sizeof(uint32_t));

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = (uint32_t)primes[i];
        if (p <= SIEVE_LIMIT) prime_index[p] = (uint32_t)(i + 1);  /* 1‑based index */
    }
}

/* backtracking partition test ------------------------------------------ */

static int cmp_desc(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return y - x;
}

static bool can_partition(const int *items, size_t m, size_t k, int target)
{
    int groupSum[64];
    for (size_t i = 0; i < k; ++i) groupSum[i] = 0;
    return backtrack_partition(0, k, target, groupSum, items, m);
}

/* test whether a number satisfies the property ------------------------ */

static bool is_good(uint64_t n)
{
    if (n < 4) return false;   /* smallest composite with property is 4 */

    uint64_t tmp = n;
    int items[128];
    size_t m = 0;

    /* factorise tmp using primes up to SIEVE_LIMIT */
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            unsigned cnt = 0;
            while (tmp % p == 0) { ++cnt; tmp /= p; }

            unsigned idx = (p <= SIEVE_LIMIT) ? prime_index[(uint32_t)p] : (unsigned)prime_pi(p);
            for (unsigned j = 0; j < cnt; ++j) {
                if (m >= sizeof(items)/sizeof(items[0])) return false;  /* safety */
                items[m++] = (int)idx;
            }
        }
    }

    /* remaining factor (prime > SIEVE_LIMIT) */
    if (tmp > 1) {
        unsigned idx = (tmp <= SIEVE_LIMIT) ? prime_index[(uint32_t)tmp] : (unsigned)prime_pi(tmp);
        items[m++] = (int)idx;   /* exponent is 1 */
    }

    if (m < 2) return false;   /* need at least two factors */

    /* total sum of indices */
    int total = 0;
    for (size_t i = 0; i < m; ++i) total += items[i];

    /* sort items descending for efficient backtracking */
    qsort(items, m, sizeof(int), cmp_desc);

    /* try all k that divide total */
    for (size_t k = 2; k <= m; ++k) {
        if (total % k != 0) continue;
        int target = total / (int)k;
        if (can_partition(items, m, k, target)) return true;
    }
    return false;
}

/* sequence cache and generator ---------------------------------------- */

static seq_t   *seq_cache   = NULL;     /* cached sequence values */
static size_t  seq_cap     = 0;         /* allocated capacity of cache */
static size_t  seq_len     = 0;         /* number of terms currently cached */
static idx_t   next_candidate = 2;      /* next integer to test */

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed > *cap) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        *ptr = (seq_t*)realloc(*ptr, newcap * sizeof(seq_t));
        if (!*ptr) abort();
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        init_primes();
    }

    while (seq_len < n) {
        if (is_good((uint64_t)next_candidate)) {
            ensure_capacity(seq_len + 1, &seq_cap, &seq_cache);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
