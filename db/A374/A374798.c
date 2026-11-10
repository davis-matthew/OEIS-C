/*
 * A374798  Inverse permutation to A374797.
 * https://oeis.org/A374798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A374797 (distinct values of the LCM
 *            table A003990 read by antidiagonals, in order of first
 *            appearance); computed by scanning antidiagonals with a hash
 *            table
 * Range:     u8 n <= 46, u16 n <= 840, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 80 terms through n = 4416 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *A374798_keys = NULL;
static uint64_t *A374798_pos = NULL;
static size_t A374798_cap = 0;
static size_t A374798_cnt = 0;
static uint64_t A374798_k = 1, A374798_i = 1;

static uint64_t A374798_hash(uint64_t x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static uint64_t A374798_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A374798_grow(void)
{
    size_t ncap = A374798_cap ? A374798_cap * 2 : 1024;
    uint64_t *nk = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    uint64_t *np = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    for (size_t t = 0; t < A374798_cap; t++) {
        if (A374798_keys[t]) {
            size_t h = (size_t)(A374798_hash(A374798_keys[t]) & (ncap - 1));
            while (nk[h]) h = (h + 1) & (ncap - 1);
            nk[h] = A374798_keys[t];
            np[h] = A374798_pos[t];
        }
    }
    free(A374798_keys);
    free(A374798_pos);
    A374798_keys = nk;
    A374798_pos = np;
    A374798_cap = ncap;
}

/* returns slot index of key, or the empty slot where it would go */
static size_t A374798_find(uint64_t key)
{
    size_t h = (size_t)(A374798_hash(key) & (A374798_cap - 1));
    while (A374798_keys[h] && A374798_keys[h] != key) h = (h + 1) & (A374798_cap - 1);
    return h;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t m = (uint64_t)n;
    if (A374798_cap == 0) A374798_grow();
    for (;;) {
        size_t h = A374798_find(m);
        if (A374798_keys[h] == m) return (seq_t)A374798_pos[h];
        /* advance one entry */
        uint64_t i = A374798_i;
        uint64_t j = A374798_k - i + 1;
        uint64_t g = A374798_gcd(i, j);
        uint64_t l = (i / g) * j;
        size_t s = A374798_find(l);
        if (!A374798_keys[s]) {
            if ((A374798_cnt + 1) * 2 > A374798_cap) {
                A374798_grow();
                s = A374798_find(l);
            }
            A374798_cnt++;
            A374798_keys[s] = l;
            A374798_pos[s] = (uint64_t)A374798_cnt;
        }
        A374798_i++;
        if (A374798_i > A374798_k) { A374798_k++; A374798_i = 1; }
    }
}
