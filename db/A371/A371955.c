/*
 * A371955  Numbers with triquanimous prime indices.
 * https://oeis.org/A371955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 491, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 493, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371955:
   Numbers with triquanimous prime indices.
   A number n is triquanimous if its prime‑indices multiset
   (obtained from exponent‑factorization of n)
   can be partitioned into three multisets with equal sums. */

/* -----------------------------------------------------------------------
   Prime generation – a fixed sieve up to 20 000 000 to obtain prime
   values and indices.  The value is sufficient to generate all
   prime indices required for the first 10 000 terms of the sequence.
   ----------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define PRIME_SIEVE_LIMIT 20000000U   /* enough for the first 10 000 terms */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

static void init_primes(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    char *is_prime = (char *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_prime) return;                     /* unlikely to happen */

    memset(is_prime, 1, PRIME_SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    /* count primes for allocation */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_cap = cnt + 10;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!prime_list) { free(is_prime); return; }

    for (uint64_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_count++] = i;

    free(is_prime);
}

static inline size_t prime_index(uint64_t p)
{
    /* binary search, 1‑based index */
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t val = prime_list[mid];
        if (val == p) return mid + 1;
        if (val < p)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return 0;          /* should never happen for correct calls */
}

/* -----------------------------------------------------------------------
   DP buffers for the triquanimous check.
   dp[i][j] is true if sums (i,j) are attainable using processed prime‑indices.
   Stored as a flat array of uint8_t.  Size is (T+1)*(T+1) for current T.
   ----------------------------------------------------------------------- */
static uint8_t *dp   = NULL;
static uint8_t *dp2  = NULL;
static size_t dp_capacity = 0;

/* -----------------------------------------------------------------------
   Determine whether n has a triquanimous multiset of prime indices.
   ----------------------------------------------------------------------- */
static bool is_triquanimous(uint64_t n)
{
    init_primes();
    if (n == 1) return false;

    /* factor n and store (index, exponent) pairs */
    /* The number of distinct prime factors of a 64‑bit integer is <64. */
    uint64_t p_indices[64];
    uint64_t p_counts[64];
    size_t pairs_len = 0;

    uint64_t temp = n;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            uint64_t exp = 0;
            while (temp % d == 0) { temp /= d; ++exp; }
            assert(pairs_len < 64);
            p_indices[pairs_len] = prime_index(d);
            p_counts[pairs_len]  = exp;
            ++pairs_len;
        }
    }
    if (temp > 1) {                   /* remaining prime factor */
        assert(pairs_len < 64);
        p_indices[pairs_len] = prime_index(temp);
        p_counts[pairs_len]  = 1;
        ++pairs_len;
    }
    if (pairs_len == 0) return false;

    /* total sum of indices (with multiplicity) */
    uint64_t total = 0;
    for (size_t i = 0; i < pairs_len; ++i)
        total += p_indices[i] * p_counts[i];

    if (total % 3 != 0) return false;
    uint32_t T = (uint32_t)(total / 3);     /* target sum for each group */

    /* allocate DP buffers if required */
    size_t need = (size_t)(T + 1) * (T + 1);
    if (dp_capacity < need) {
        free(dp);
        free(dp2);
        dp  = (uint8_t *)malloc(need);
        dp2 = (uint8_t *)malloc(need);
        if (!dp || !dp2) { dp_capacity = 0; return false; }
        dp_capacity = need;
    }

    memset(dp, 0, need);
    dp[0] = 1;                           /* sums (0,0) reachable */
    const size_t stride = T + 1;

    /* process each (index, exponent) pair */
    for (size_t i = 0; i < pairs_len; ++i) {
        uint32_t w = (uint32_t)p_indices[i];
        uint32_t c = (uint32_t)p_counts[i];
        memset(dp2, 0, need);

        for (uint32_t sum0 = 0; sum0 <= T; ++sum0) {
            size_t base = (size_t)sum0 * stride;
            for (uint32_t sum1 = 0; sum1 <= T; ++sum1) {
                if (!dp[base + sum1]) continue;
                uint32_t max_k = (T - sum0) / w;
                if (max_k > c) max_k = c;
                for (uint32_t k = 0; k <= max_k; ++k) {
                    uint32_t new0 = sum0 + k * w;
                    uint32_t max_l = (T - sum1) / w;
                    if (max_l > c - k) max_l = c - k;
                    for (uint32_t l = 0; l <= max_l; ++l) {
                        uint32_t new1 = sum1 + l * w;
                        dp2[(size_t)new0 * stride + new1] = 1;
                    }
                }
            }
        }
        /* swap buffers for next iteration */
        uint8_t *tmp = dp; dp = dp2; dp2 = tmp;
    }

    return dp[(size_t)T * stride + T] != 0;
}

/* -----------------------------------------------------------------------
   Cached storage of sequence terms for fast repeated queries.
   ----------------------------------------------------------------------- */
static seq_t *term_cache = NULL;
static size_t cache_size  = 0;    /* number of valid terms in cache */
static size_t cache_cap   = 0;
static uint64_t next_scan = 1;    /* next integer to test for triquanomy */

/* -----------------------------------------------------------------------
   Public sequence function.
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */

    while (cache_size < (size_t)n) {
        if (is_triquanimous(next_scan)) {     /* term found */
            /* grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *new_arr = (seq_t *)realloc(term_cache,
                                                   new_cap * sizeof(seq_t));
                if (!new_arr) exit(1);          /* allocation failed */
                term_cache = new_arr;
                cache_cap  = new_cap;
            }
            term_cache[cache_size++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return term_cache[n - 1];
}
