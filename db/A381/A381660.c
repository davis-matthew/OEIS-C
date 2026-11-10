/*
 * A381660  The value k such that n = a(j) = a(j+k) = a(j+2*k) in A381658.
 * https://oeis.org/A381660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where the unique triple n = A381658(j) = A381658(j+k) =
 *            A381658(j+2k) occurs; computed by generating A381658 greedily
 *            (search), using per-value bitsets of "completion" positions 2j -
 *            i to test the triple rule in O(1)
 * Range:     u8 n <= 25, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Validated: standard, pass, 103 terms through n = 246 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t *pos;      /* sorted positions where this value occurs */
    uint32_t len, cap;
    uint32_t base;      /* first position; bit offsets are t - base */
    uint64_t *f1;       /* bit set: at least one pair completes a 3-AP at t */
    uint64_t *f2;       /* bit set: at least two pairs complete (only before triple) */
    size_t words;
    uint32_t k;         /* triple spacing, 0 if none yet */
} A381660_val;

static A381660_val *A381660_vals = NULL;
static size_t A381660_vals_cap = 0;
static uint32_t A381660_gen = 0;

static void A381660_ensure_vals(size_t v)
{
    if (v < A381660_vals_cap) return;
    size_t nc = A381660_vals_cap ? A381660_vals_cap : 256;
    while (nc <= v) nc <<= 1;
    A381660_vals = (A381660_val *)realloc(A381660_vals, nc * sizeof(A381660_val));
    for (size_t i = A381660_vals_cap; i < nc; ++i) {
        A381660_vals[i].pos = NULL;
        A381660_vals[i].len = 0;
        A381660_vals[i].cap = 0;
        A381660_vals[i].base = 0;
        A381660_vals[i].f1 = NULL;
        A381660_vals[i].f2 = NULL;
        A381660_vals[i].words = 0;
        A381660_vals[i].k = 0;
    }
    A381660_vals_cap = nc;
}

static void A381660_ensure_bits(A381660_val *v, size_t bit)
{
    size_t need = bit / 64 + 1;
    if (need <= v->words) return;
    size_t nw = v->words ? v->words : 64;
    while (nw < need) nw <<= 1;
    v->f1 = (uint64_t *)realloc(v->f1, nw * sizeof(uint64_t));
    memset(v->f1 + v->words, 0, (nw - v->words) * sizeof(uint64_t));
    if (v->k == 0) {
        v->f2 = (uint64_t *)realloc(v->f2, nw * sizeof(uint64_t));
        memset(v->f2 + v->words, 0, (nw - v->words) * sizeof(uint64_t));
    }
    v->words = nw;
}

static bool A381660_test(const uint64_t *b, size_t words, size_t bit)
{
    if (b == NULL || bit / 64 >= words) return false;
    return (b[bit / 64] >> (bit % 64)) & 1u;
}

static bool A381660_bsearch(const uint32_t *arr, uint32_t len, uint32_t t)
{
    uint32_t lo = 0, hi = len;
    while (lo < hi) {
        uint32_t mid = (lo + hi) >> 1;
        if (arr[mid] < t) lo = mid + 1; else hi = mid;
    }
    return lo < len && arr[lo] == t;
}

static void A381660_step(void)
{
    uint32_t p = A381660_gen + 1;
    size_t v = 1;
    A381660_val *info;
    for (;; ++v) {
        A381660_ensure_vals(v);
        info = &A381660_vals[v];
        if (info->len == 0) break;
        size_t off = (size_t)(p - info->base);
        if (info->k != 0) {
            if (!A381660_test(info->f1, info->words, off)) break;
        } else {
            if (!A381660_test(info->f2, info->words, off)) break;
        }
    }

    if (info->len == 0) {
        info->base = p;
    } else if (info->k == 0 &&
               A381660_test(info->f1, info->words, (size_t)(p - info->base))) {
        /* a triple is formed now: find its spacing */
        for (uint32_t i = info->len; i-- > 0;) {
            uint32_t j = info->pos[i];
            if ((uint64_t)2 * j <= p) break;
            uint32_t q = 2 * j - p;
            if (A381660_bsearch(info->pos, i, q)) {
                info->k = p - j;
                break;
            }
        }
        free(info->f2);
        info->f2 = NULL;
    }

    /* register completion targets 2p - q */
    if (info->len > 0)
        A381660_ensure_bits(info, (size_t)(2 * p - info->pos[0] - info->base));
    for (uint32_t i = 0; i < info->len; ++i) {
        size_t off = (size_t)(2 * p - info->pos[i] - info->base);
        uint64_t m = (uint64_t)1 << (off % 64);
        size_t w = off / 64;
        if (info->k != 0) {
            info->f1[w] |= m;
        } else if (info->f1[w] & m) {
            info->f2[w] |= m;
        } else {
            info->f1[w] |= m;
        }
    }

    if (info->len == info->cap) {
        uint32_t nc = info->cap ? info->cap * 2 : 8;
        info->pos = (uint32_t *)realloc(info->pos, (size_t)nc * sizeof(uint32_t));
        info->cap = nc;
    }
    info->pos[info->len++] = p;
    A381660_gen = p;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    size_t v = (size_t)n;
    A381660_ensure_vals(v);
    while (A381660_vals[v].k == 0) {
        A381660_step();
        A381660_ensure_vals(v);
    }
    return (seq_t)A381660_vals[v].k;
}
