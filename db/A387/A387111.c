/*
 * A387111  Number of ways to choose a sequence of distinct positive integers,
 *          one in the initial interval of each prime index of n.
 * https://oeis.org/A387111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over the sorted list of prime indices of n of
 *            (index[i] - (i-1)), with a(1)=1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *g_primes = NULL;        /* list of primes up to current limit */
static size_t    g_prime_count = 0;      /* number of primes in the list */
static uint32_t  g_prime_limit = 0;      /* maximum n for which the list is built */

/* Build or extend a sieve of Eratosthenes up to at least 'limit'. */
static void build_prime_sieve(uint32_t limit)
{
    if (limit <= g_prime_limit)
        return;                         /* already built */

    /* free previous data */
    free(g_primes);

    /* temporary array of composites */
    uint8_t *is_comp = (uint8_t*)calloc((size_t)limit + 1U, 1U);
    if (!is_comp)
        exit(1);

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2U; (uint64_t)i * (uint64_t)i <= (uint64_t)limit; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1U;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2U; i <= limit; ++i)
        if (!is_comp[i])
            ++cnt;

    /* store primes */
    g_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!g_primes)
        exit(1);
    g_prime_count = cnt;
    size_t pos = 0;
    for (uint32_t i = 2U; i <= limit; ++i)
        if (!is_comp[i])
            g_primes[pos++] = i;

    free(is_comp);
    g_prime_limit = limit;
}

/* Binary search to find the 1‑based index of prime 'p' in the global prime list. */
static uint32_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = g_prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (g_primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo should point to 'p' */
    return (uint32_t)(lo + 1);           /* 1‑based index */
}

/* Count the number of ways for a given n. */
seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    uint32_t N = (uint32_t)n;
    build_prime_sieve(N);

    /* Temporary array for prime indices (sufficiently large for small n). */
    uint32_t idxs[64];
    size_t idx_cnt = 0;

    uint64_t tmp = (uint64_t)n;

    /* Factor with precomputed primes. */
    for (size_t k = 0; k < g_prime_count; ++k) {
        uint32_t p = g_primes[k];
        if ((uint64_t)p * (uint64_t)p > tmp)
            break;
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            uint32_t idx = (uint32_t)(k + 1); /* 1‑based index of p */
            for (uint32_t j = 0; j < exp; ++j) {
                if (idx_cnt < sizeof(idxs)/sizeof(idxs[0]))
                    idxs[idx_cnt++] = idx;
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        uint32_t rem = (uint32_t)tmp;
        uint32_t idx;
        if (rem <= g_prime_limit)
            idx = prime_index(rem);
        else {
            /* In case the remainder exceeds current sieve limit,
               rebuild the sieve (should not happen for small n). */
            build_prime_sieve(rem);
            idx = prime_index(rem);
        }
        if (idx_cnt < sizeof(idxs)/sizeof(idxs[0]))
            idxs[idx_cnt++] = idx;
    }

    /* Compute the product over sorted indices. */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < idx_cnt; ++i) {
        uint32_t diff = idxs[i] - (uint32_t)i;
        if (diff == 0)
            return (seq_t)0;
        prod *= (seq_t)diff;
    }

    return prod;
}
