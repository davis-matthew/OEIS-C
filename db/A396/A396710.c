/*
 * A396710  Number of unordered factorizations of n into exactly k squarefree
 *          factors, where k is the maximum exponent in the prime
 *          factorization of n.
 * https://oeis.org/A396710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into exactly k
 *            squarefree factors, where k is the maximum exponent in the prime
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

typedef struct {
    uint64_t *data;
    size_t size;
    size_t cap;
} KeyArr;

static void init_keyarr(KeyArr *arr) {
    arr->size = 0;
    arr->cap = 16;
    arr->data = (uint64_t *)malloc(arr->cap * sizeof(uint64_t));
}

static void add_key(KeyArr *arr, uint64_t key) {
    if (arr->size == arr->cap) {
        arr->cap *= 2;
        arr->data = (uint64_t *)realloc(arr->data, arr->cap * sizeof(uint64_t));
    }
    arr->data[arr->size++] = key;
}

static void free_keyarr(KeyArr *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = arr->cap = 0;
}

static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Recursive enumeration of assignments of primes to factors */
static void dfs(int primeIdx, int totalPrimes, int k,
                const uint64_t remPr[], const int remEx[],
                uint64_t factors[], KeyArr *keys) {
    if (primeIdx == totalPrimes) {
        uint64_t tmp[20];
        for (int i = 0; i < k; ++i) tmp[i] = factors[i];
        /* insertion sort for the tiny array */
        for (int i = 1; i < k; ++i) {
            uint64_t v = tmp[i];
            int j = i - 1;
            while (j >= 0 && tmp[j] > v) {
                tmp[j + 1] = tmp[j];
                --j;
            }
            tmp[j + 1] = v;
        }
        /* encode as hash key */
        const uint64_t BASE = 10007;
        uint64_t key = 0;
        for (int i = 0; i < k; ++i)
            key = key * BASE + tmp[i];
        add_key(keys, key);
        return;
    }

    int e = remEx[primeIdx];
    uint64_t p = remPr[primeIdx];
    if (e == 0) {
        dfs(primeIdx + 1, totalPrimes, k, remPr, remEx, factors, keys);
        return;
    }

    int maxMask = 1 << k;
    for (int mask = 0; mask < maxMask; ++mask) {
        if (__builtin_popcount((unsigned)mask) != e)
            continue;
        int j;
        for (j = 0; j < k; ++j) {
            if (mask & (1 << j))
                factors[j] *= p;
        }
        dfs(primeIdx + 1, totalPrimes, k, remPr, remEx, factors, keys);
        for (j = 0; j < k; ++j) {
            if (mask & (1 << j))
                factors[j] /= p;
        }
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Determine k (maximum exponent) */
    int k = 0;
    for (size_t i = 0; i < cnt; ++i)
        if ((int)exps[i] > k) k = (int)exps[i];

    /* Find one prime that attains the maximum exponent */
    int idx_k = -1;
    for (size_t i = 0; i < cnt; ++i)
        if ((int)exps[i] == k) {
            idx_k = (int)i;
            break;
        }

    /* Remaining primes after removing p^k */
    int remCnt = 0;
    uint64_t remPr[20];
    int remEx[20];
    for (size_t i = 0; i < cnt; ++i) {
        if ((int)i == idx_k) continue;
        remPr[remCnt] = primes[i];
        remEx[remCnt] = (int)exps[i];
        ++remCnt;
    }

    uint64_t factors[20];
    for (int i = 0; i < k; ++i) factors[i] = 1;

    KeyArr keys;
    init_keyarr(&keys);
    dfs(0, remCnt, k, remPr, remEx, factors, &keys);
    qsort(keys.data, keys.size, sizeof(uint64_t), cmp_uint64);

    seq_t count = 0;
    for (size_t i = 0; i < keys.size; ++i) {
        if (i == 0 || keys.data[i] != keys.data[i - 1])
            ++count;
    }

    free_keyarr(&keys);
    return count;
}
