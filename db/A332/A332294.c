/*
 * A332294  Number of unimodal permutations of a multiset whose multiplicities
 *          are the prime indices of n.
 * https://oeis.org/A332294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{p^e∥n} (index(p)+1)^e) / (min_index+1)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where index(p) is the 1‑based index of prime p in the list of primes */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static prime table up to 10000 (enough for all n ≤ 10000) */
static uint32_t prime_tbl[2000];
static size_t prime_cnt = 0;
static bool prime_tbl_ready = false;

/* initialise prime table once */
static void init_prime_tbl(void)
{
    if (prime_tbl_ready)
        return;

    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto((idx_t)10000, &tmp, &tmp_cnt);
    prime_cnt = tmp_cnt;
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_tbl[i] = (uint32_t)tmp[i];
    free(tmp);
    prime_tbl_ready = true;
}

/* return 1‑based index of prime p, 0 if not found (should not happen) */
static uint32_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t mp = prime_tbl[mid];
        if (mp == (uint32_t)p)
            return (uint32_t)(mid + 1);
        if ((uint64_t)mp < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* not found, should not occur for valid inputs */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    init_prime_tbl();

    seq_t prod = 1;
    uint32_t min_idx = 0;
    uint64_t temp = (uint64_t)n;

    /* trial division by primes up to sqrt(temp) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p * p > temp)
            break;
        if (temp % p != 0)
            continue;

        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        uint32_t idx_p = prime_index(p);   /* 1‑based index */
        seq_t base = (seq_t)(idx_p + 1);

        for (unsigned j = 0; j < exp; ++j)
            prod *= base;

        if (min_idx == 0 || idx_p < min_idx)
            min_idx = idx_p;
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        uint32_t idx_p = prime_index(temp);
        seq_t base = (seq_t)(idx_p + 1);
        prod *= base;
        if (min_idx == 0 || idx_p < min_idx)
            min_idx = idx_p;
    }

    /* divide by (min_idx+1); division is exact */
    prod /= (seq_t)(min_idx + 1);
    return prod;
}
