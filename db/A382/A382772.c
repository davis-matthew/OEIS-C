/*
 * A382772  Set of positions of first appearances in A382771 (permutations of
 *          prime indices with distinct run-lengths).
 * https://oeis.org/A382772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th position k (in increasing order) at which A382771(k)
 *            takes a value not seen at any earlier position; computed by
 *            scanning k and caching A382771 by exponent signature
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

#define A382772_SIGCAP 100000
#define A382772_SEENCAP 100000
#define A382772_KEYLEN 16

static void A382771_dfs(int *counts, int k, int last, uint64_t used_mask,
                        int remaining, seq_t *total)
{
    if (remaining == 0) {
        ++(*total);
        return;
    }
    for (int i = 0; i < k; ++i) {
        if (counts[i] == 0 || i == last)
            continue;
        int c = counts[i];
        for (int len = 1; len <= c; ++len) {
            uint64_t bit = ((uint64_t)1) << (len - 1);
            if (used_mask & bit)
                continue;
            counts[i] = c - len;
            A382771_dfs(counts, k, i, used_mask | bit, remaining - len, total);
            counts[i] = c;
        }
    }
}

static unsigned char *A382772_keys = 0;
static seq_t *A382772_vals = 0;
static size_t A382772_ncache = 0;
static seq_t *A382772_seen = 0;
static size_t A382772_nseen = 0;
static idx_t *A382772_pos = 0;
static size_t A382772_npos = 0;
static idx_t A382772_next = 1;

static seq_t A382772_value(idx_t n)
{
    int ex[A382772_KEYLEN];
    int k = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) { m /= p; ++cnt; }
            ex[k++] = cnt;
        }
    }
    if (m > 1) ex[k++] = 1;
    /* sort descending */
    for (int i = 1; i < k; ++i) {
        int v = ex[i], j = i - 1;
        while (j >= 0 && ex[j] < v) { ex[j + 1] = ex[j]; --j; }
        ex[j + 1] = v;
    }
    unsigned char key[A382772_KEYLEN];
    for (int i = 0; i < A382772_KEYLEN; ++i)
        key[i] = (unsigned char)(i < k ? ex[i] : 0);
    for (size_t c = 0; c < A382772_ncache; ++c) {
        if (memcmp(A382772_keys + c * A382772_KEYLEN, key, A382772_KEYLEN) == 0)
            return A382772_vals[c];
    }
    seq_t result = 0;
    if (k == 0) {
        result = 1;
    } else {
        int total = 0;
        for (int i = 0; i < k; ++i) total += ex[i];
        A382771_dfs(ex, k, k, 0, total, &result);
    }
    if (A382772_ncache < A382772_SIGCAP) {
        memcpy(A382772_keys + A382772_ncache * A382772_KEYLEN, key, A382772_KEYLEN);
        A382772_vals[A382772_ncache] = result;
        ++A382772_ncache;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (!A382772_keys) {
        A382772_keys = (unsigned char *)malloc((size_t)A382772_SIGCAP * A382772_KEYLEN);
        A382772_vals = mem_new(seq_t, A382772_SIGCAP);
        A382772_seen = mem_new(seq_t, A382772_SEENCAP);
        A382772_pos = (idx_t *)malloc(sizeof(idx_t) * A382772_SEENCAP);
    }
    while (A382772_npos < (size_t)n) {
        idx_t k = A382772_next++;
        seq_t v = A382772_value(k);
        bool found = false;
        for (size_t i = 0; i < A382772_nseen; ++i) {
            if (A382772_seen[i] == v) { found = true; break; }
        }
        if (!found && A382772_nseen < A382772_SEENCAP) {
            A382772_seen[A382772_nseen++] = v;
            A382772_pos[A382772_npos++] = k;
        }
    }
    return (seq_t)A382772_pos[n - 1];
}
