/*
 * A359215  Number of terms in S(n) that did not appear in previous
 *          trajectories, where S(n) is the trajectory of the mappings of
 *          x->A359194(x) starting with n and stopping when 0 is reached, -1
 *          if 0 is never reached.
 * https://oeis.org/A359215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- iterate x -> A359194(x) = 2^k - 1 - 3x (k = bit length
 *            of 3x) from n, using built-in multi-precision values, and count
 *            the terms not seen in S(0..n-1); stop at 0 or at the first term
 *            already seen. a(0) = 0
 * Range:     u8 n <= 27, u16 n <= 226, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9355 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---------- multi-precision trajectory values (uint32 limbs, little endian) ---------- */

static uint32_t *A359215_bufA = NULL;
static uint32_t *A359215_bufB = NULL;
static size_t A359215_bcap = 0;

static void A359215_ensure(size_t need)
{
    if (need <= A359215_bcap) return;
    size_t nc = A359215_bcap ? A359215_bcap : 64;
    while (nc < need) nc <<= 1;
    A359215_bufA = (uint32_t *)realloc(A359215_bufA, nc * sizeof(uint32_t));
    A359215_bufB = (uint32_t *)realloc(A359215_bufB, nc * sizeof(uint32_t));
    A359215_bcap = nc;
}

static int A359215_bitlen32(uint32_t v)
{
    int b = 0;
    while (v) { v >>= 1; ++b; }
    return b;
}

/* y = A359194(x); x has xl > 0 limbs, normalized. Returns length of y (0 if y == 0). */
static size_t A359215_step(const uint32_t *x, size_t xl, uint32_t *y)
{
    uint64_t carry = 0;
    size_t ml = xl;
    for (size_t i = 0; i < xl; ++i) {
        uint64_t t = (uint64_t)x[i] * 3u + carry;
        y[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) y[ml++] = (uint32_t)carry;
    /* m = 3x now in y[0..ml-1], top limb nonzero */
    int tb = A359215_bitlen32(y[ml - 1]);
    for (size_t i = 0; i < ml; ++i) y[i] = ~y[i];
    if (tb < 32) y[ml - 1] &= (uint32_t)((1u << tb) - 1u);
    while (ml > 0 && y[ml - 1] == 0) --ml;
    return ml;
}

/* ---------- 128-bit fingerprint set of visited values ---------- */

static uint64_t A359215_mix(uint64_t z)
{
    z += 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static void A359215_fp(const uint32_t *x, size_t xl, uint64_t *h1, uint64_t *h2)
{
    uint64_t a = 0x0123456789ABCDEFULL ^ (uint64_t)xl;
    uint64_t b = 0xFEDCBA9876543210ULL + (uint64_t)xl * 0x100000001B3ULL;
    for (size_t i = 0; i < xl; ++i) {
        a = A359215_mix(a ^ (uint64_t)x[i]);
        b = A359215_mix((b + (uint64_t)x[i] * 0xC2B2AE3D27D4EB4FULL) ^ 0x5851F42D4C957F2DULL);
    }
    if (a == 0 && b == 0) a = 1;
    *h1 = a;
    *h2 = b;
}

static uint64_t *A359215_tab = NULL;   /* 2 words per slot, (0,0) = empty */
static size_t A359215_cap = 0;
static size_t A359215_cnt = 0;

static void A359215_put(uint64_t *tab, size_t cap, uint64_t h1, uint64_t h2)
{
    size_t idx = (size_t)(h1 ^ (h2 >> 7)) & (cap - 1);
    while (tab[2 * idx] != 0 || tab[2 * idx + 1] != 0) idx = (idx + 1) & (cap - 1);
    tab[2 * idx] = h1;
    tab[2 * idx + 1] = h2;
}

static void A359215_grow(size_t nc)
{
    uint64_t *nt = (uint64_t *)calloc(nc * 2, sizeof(uint64_t));
    for (size_t i = 0; i < A359215_cap; ++i) {
        uint64_t h1 = A359215_tab[2 * i], h2 = A359215_tab[2 * i + 1];
        if (h1 != 0 || h2 != 0) A359215_put(nt, nc, h1, h2);
    }
    free(A359215_tab);
    A359215_tab = nt;
    A359215_cap = nc;
}

/* true if x was newly inserted */
static bool A359215_insert(const uint32_t *x, size_t xl)
{
    uint64_t h1, h2;
    A359215_fp(x, xl, &h1, &h2);
    if (A359215_cap == 0) A359215_grow(1024);
    if ((A359215_cnt + 1) * 2 >= A359215_cap) A359215_grow(A359215_cap * 2);
    size_t idx = (size_t)(h1 ^ (h2 >> 7)) & (A359215_cap - 1);
    while (A359215_tab[2 * idx] != 0 || A359215_tab[2 * idx + 1] != 0) {
        if (A359215_tab[2 * idx] == h1 && A359215_tab[2 * idx + 1] == h2) return false;
        idx = (idx + 1) & (A359215_cap - 1);
    }
    A359215_tab[2 * idx] = h1;
    A359215_tab[2 * idx + 1] = h2;
    A359215_cnt++;
    return true;
}

/* ---------- memo of a(n) ---------- */

static seq_t *A359215_memo = NULL;
static size_t A359215_len = 0;
static size_t A359215_mcap = 0;

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (A359215_len == 0) {
        A359215_ensure(64);
        A359215_insert(A359215_bufA, 0);          /* the value 0 (S(0)) */
        A359215_mcap = 16;
        A359215_memo = mem_new(seq_t, A359215_mcap);
        A359215_memo[0] = (seq_t)0;
        A359215_len = 1;
    }
    if (N >= A359215_len) {
        if (N + 1 > A359215_mcap) {
            size_t nc = A359215_mcap;
            while (nc < N + 1) nc <<= 1;
            A359215_memo = mem_resize(seq_t, A359215_memo, nc);
            A359215_mcap = nc;
        }
        for (size_t i = A359215_len; i <= N; ++i) {
            A359215_ensure(8);
            uint32_t *cur = A359215_bufA;
            uint32_t *nxt = A359215_bufB;
            uint64_t v = (uint64_t)i;
            size_t cl = 0;
            while (v) { cur[cl++] = (uint32_t)v; v >>= 32; }
            unsigned long long count = 0;
            while (cl > 0) {
                if (!A359215_insert(cur, cl)) break;
                count++;
                A359215_ensure(cl + 4);
                cur = A359215_bufA; nxt = A359215_bufB;
                cl = A359215_step(cur, cl, nxt);
                /* swap buffers */
                uint32_t *t = A359215_bufA;
                A359215_bufA = A359215_bufB;
                A359215_bufB = t;
                cur = A359215_bufA;
            }
            A359215_memo[i] = (seq_t)count;
        }
        A359215_len = N + 1;
    }
    return A359215_memo[N];
}
