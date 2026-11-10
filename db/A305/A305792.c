/*
 * A305792  a(n) = Product_{d|n, d<n} prime(A286622(d)-1).
 * https://oeis.org/A305792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} prime(A286622(d)-1)
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 167, u64 n <= 419,
 *            u128 n <= 2519, big any n
 * Validated: standard, pass, 96 terms through n = 7348 (2026-11-08)
 * Helpers:   mem, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/nth_prime.h"

static void A286622_multiset(uint64_t n, size_t *out_len, uint8_t *out_lens)
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
    uint64_t label;
} A286622_Key;

static uint64_t *A286622_cache = NULL;
static size_t A286622_cap = 0;
static idx_t A286622_maxgen = 0;
static A286622_Key *A286622_keys = NULL;
static size_t A286622_key_cap = 0;
static size_t A286622_key_cnt = 0;
static uint64_t A286622_next = 1;

static void A286622_ensure_cache(idx_t n)
{
    if (A286622_cache && (size_t)n < A286622_cap) return;
    size_t new_cap = A286622_cap ? A286622_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(A286622_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    for (size_t i = A286622_cap; i < new_cap; ++i) tmp[i] = 0;
    A286622_cache = tmp;
    A286622_cap = new_cap;
}

static void A286622_ensure_keys(void)
{
    if (A286622_key_cnt < A286622_key_cap) return;
    size_t new_cap = A286622_key_cap ? A286622_key_cap << 1 : 8;
    A286622_Key *tmp = (A286622_Key *)realloc(A286622_keys, new_cap * sizeof(A286622_Key));
    if (!tmp) abort();
    A286622_keys = tmp;
    A286622_key_cap = new_cap;
}

static void A286622_generate(idx_t n)
{
    for (idx_t i = A286622_maxgen + 1; i <= n; ++i) {
        size_t len;
        uint8_t tl[64];
        A286622_multiset((uint64_t)i, &len, tl);
        uint64_t label = 0;
        for (size_t k = 0; k < A286622_key_cnt; ++k) {
            if (A286622_keys[k].len != len) continue;
            if (len == 0 || memcmp(A286622_keys[k].lens, tl, len) == 0) {
                label = A286622_keys[k].label;
                break;
            }
        }
        if (label == 0) {
            A286622_ensure_keys();
            A286622_Key *kp = &A286622_keys[A286622_key_cnt++];
            kp->len = len;
            if (len) {
                kp->lens = (uint8_t *)malloc(len);
                if (!kp->lens) abort();
                memcpy(kp->lens, tl, len);
            } else kp->lens = NULL;
            kp->label = A286622_next++;
            label = kp->label;
        }
        A286622_cache[(size_t)i] = label;
    }
    A286622_maxgen = n;
}

static uint64_t A286622_a(idx_t n)
{
    if (!A286622_cache) {
        A286622_ensure_cache(n);
        A286622_cache[0] = 1;
        A286622_next = 2;
        A286622_maxgen = 0;
    }
    if (n > A286622_maxgen) {
        A286622_ensure_cache(n);
        A286622_generate(n);
    }
    return A286622_cache[(size_t)n];
}

seq_t a(idx_t n)
{
    seq_t r = 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d) continue;
        idx_t e = n / d;
        if (d < n) {
            r *= (seq_t)nth_prime((idx_t)(A286622_a(d) - 1));
        }
        if (e != d && e < n) {
            r *= (seq_t)nth_prime((idx_t)(A286622_a(e) - 1));
        }
    }
    return r;
}
