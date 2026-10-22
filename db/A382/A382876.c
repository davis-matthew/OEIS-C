/*
 * A382876  Number of ways to permute the prime indices of n so that the
 *          run-sums are all different.
 * https://oeis.org/A382876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   valid_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of permutations of the multiset of prime indices
 * of n whose run‑sum partition has all distinct sums */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/valid_perm.h"

/* ---------- prime table ---------- */
static uint32_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Build all primes up to 100000 (enough for n <= 10000). */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;                      /* already built */

    const size_t limit = 100000;
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime)
        exit(1);

    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        exit(1);
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    free(is_prime);
}

/* Return 1‑based index of prime p in prime_list (or 0 if not found). */
static size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t val = prime_list[mid];
        if (val == p)
            return mid + 1;
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* Factorize n (n <= 10^4).  Output primes and exponents. */
static void factor_uint64(uint64_t n,
                          uint32_t *pf, uint32_t *pe, size_t *cnt)
{
    init_primes();
    *cnt = 0;
    for (size_t i = 0;
         prime_list[i] * (uint64_t)prime_list[i] <= n;
         ++i)
    {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[(*cnt)] = p;
            pe[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) { /* remaining prime factor > sqrt(original n) */
        pf[(*cnt)] = (uint32_t)n;
        pe[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* Recursive enumeration of distinct permutations of a multiset
 * described by values[] / counts[].  result accumulates the number
 * of valid permutations. */
static void generate(int m, int distinct,
                     const uint32_t *values, int *cnt,
                     uint32_t *perm, int pos, seq_t *result)
{
    if (pos == m) {
        if (valid_perm(perm, m))
            ++(*result);
        return;
    }
    for (int i = 0; i < distinct; ++i) {
        if (cnt[i] == 0)
            continue;
        --cnt[i];
        perm[pos] = values[i];
        generate(m, distinct, values, cnt, perm, pos + 1, result);
        ++cnt[i];
    }
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* By definition a(1)=1 (empty multiset). */
    if (n == 1)
        return (seq_t)1;

    /* Factor n. */
    static uint32_t pf[32], pe[32];
    size_t fcnt = 0;
    factor_uint64((uint64_t)n, pf, pe, &fcnt);

    /* Build multiset of prime indices: each prime factor p of exponent e
     * contributes e copies of its index in the list. */
    const int MAX_M = 32;
    uint32_t idxs[MAX_M];
    int total = 0;
    for (size_t i = 0; i < fcnt; ++i) {
        uint32_t p = pf[i];
        int e = pe[i];
        size_t ix = prime_index(p);
        if (ix == 0)
            return (seq_t)0;        /* should not happen for valid n */
        for (int k = 0; k < e; ++k) {
            if (total >= MAX_M)
                break;              /* safety */
            idxs[total++] = (uint32_t)ix;
        }
    }

    if (total == 0)
        return (seq_t)1; /* only possible for n=1, handled above */

    /* Sort idxs to count multiplicities.  Simple bubble sort (size <= 13). */
    for (int i = 0; i < total - 1; ++i)
        for (int j = i + 1; j < total; ++j)
            if (idxs[i] > idxs[j]) {
                uint32_t tmp = idxs[i];
                idxs[i] = idxs[j];
                idxs[j] = tmp;
            }

    /* Compress into distinct values and counts. */
    uint32_t values[20];
    int counts[20];
    int distinct = 0;
    for (int i = 0; i < total; ) {
        uint32_t val = idxs[i];
        int j = i;
        while (j < total && idxs[j] == val)
            ++j;
        values[distinct] = val;
        counts[distinct] = j - i;
        ++distinct;
        i = j;
    }

    /* Enumerate all distinct permutations and count valid ones. */
    seq_t result = 0;
    uint32_t perm[MAX_M];
    generate(total, distinct, values, counts, perm, 0, &result);

    return result;
}
