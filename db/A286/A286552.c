/*
 * A286552  Ordinal transform of A286622, or equally, of A278222.
 * https://oeis.org/A286552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k in 0..n with A286622(k) = A286622(n) (ordinal
 *            transform, counting from 1)
 * Range:     u8 n <= 3204, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 129 terms through n = 8750 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static void A286622_runs(uint64_t n, size_t *out_len, uint8_t *out_lens)
{
    uint8_t tmp[64];
    size_t len = 0;
    if (n == 0) { *out_len = 0; return; }
    uint64_t m = n;
    while (m) {
        while ((m & 1ULL) == 0) m >>= 1;
        size_t cnt = 0;
        while ((m & 1ULL) == 1) { ++cnt; m >>= 1; }
        tmp[len++] = (uint8_t)cnt;
    }
    for (size_t i = 1; i < len; ++i) {
        uint8_t val = tmp[i];
        size_t j = i;
        while (j > 0 && tmp[j - 1] > val) { tmp[j] = tmp[j - 1]; --j; }
        tmp[j] = val;
    }
    memcpy(out_lens, tmp, len);
    *out_len = len;
}

typedef struct {
    size_t len;
    uint8_t *lens;
    seq_t label;
} A286622_Key;

static seq_t *A286622_cache = NULL;
static size_t A286622_cap = 0;
static idx_t A286622_max = 0;
static A286622_Key *A286622_keys = NULL;
static size_t A286622_kcap = 0;
static size_t A286622_kcnt = 0;
static seq_t A286622_next = 1;

static void A286622_ensure_cache(idx_t n)
{
    if (A286622_cache && (size_t)n < A286622_cap) return;
    size_t new_cap = A286622_cap ? A286622_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A286622_cache, new_cap);
    if (!tmp) abort();
    for (size_t i = A286622_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    A286622_cache = tmp;
    A286622_cap = new_cap;
}

static void A286622_ensure_keys(void)
{
    if (A286622_kcnt < A286622_kcap) return;
    size_t new_cap = A286622_kcap ? A286622_kcap << 1 : 8;
    A286622_Key *tmp = mem_resize(A286622_Key, A286622_keys, new_cap);   /* typed: the key holds a seq_t */
    if (!tmp) abort();
    A286622_keys = tmp;
    A286622_kcap = new_cap;
}

static void A286622_generate(idx_t n)
{
    for (idx_t i = A286622_max + 1; i <= n; ++i) {
        size_t len = 0;
        uint8_t tl[64];
        A286622_runs((uint64_t)i, &len, tl);
        seq_t label = 0;
        for (size_t k = 0; k < A286622_kcnt; ++k) {
            if (A286622_keys[k].len != len) continue;
            if (len == 0 || memcmp(A286622_keys[k].lens, tl, len) == 0) {
                label = A286622_keys[k].label;
                break;
            }
        }
        if (label == 0) {
            A286622_ensure_keys();
            A286622_Key *kp = &A286622_keys[A286622_kcnt++];
            kp->len = len;
            if (len) {
                kp->lens = (uint8_t *)malloc(len);
                if (!kp->lens) abort();
                memcpy(kp->lens, tl, len);
            } else {
                kp->lens = NULL;
            }
            kp->label = A286622_next++;
            label = kp->label;
        }
        A286622_cache[(size_t)i] = label;
    }
    A286622_max = n;
}

static seq_t A286622_a(idx_t n)
{
    if (!A286622_cache) {
        A286622_ensure_cache(n);
        A286622_cache[0] = (seq_t)1;
        A286622_next = (seq_t)2;
        A286622_max = 0;
    }
    if (n > A286622_max) {
        A286622_ensure_cache(n);
        A286622_generate(n);
    }
    return A286622_cache[(size_t)n];
}

seq_t a(idx_t n)
{
    seq_t target = A286622_a(n);
    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k) {
        if (A286622_a(k) == target) ++count;
    }
    return count;
}
