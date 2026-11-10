/*
 * A275840  a(n) = A273670(n) - A256450(n).
 * https://oeis.org/A275840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A273670(n) - A256450(n); both terms are computed by a
 *            search (scan integers, test factorial-base digits)
 * Range:     u8 n <= 242, u16 n <= 35279, u32 n <= 35279, u64 n <= 35279,
 *            u128 n <= 35279, big any n
 * Validated: standard, pass, 124 terms through n = 9412 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A273670_has_max(seq_t x)
{
    seq_t n = x;
    seq_t k = 1;
    while (n > 0) {
        seq_t mod = k + 1;
        seq_t d = n % mod;
        if (d == k) return true;
        n = n / mod;
        k = k + 1;
    }
    return false;
}

static seq_t *A273670_cache = NULL;
static idx_t A273670_csz = 0;
static idx_t A273670_cap = 0;

static void A273670_grow(idx_t need)
{
    if (need <= A273670_cap) return;
    idx_t newcap = A273670_cap ? A273670_cap << 1 : 32;
    while (newcap < need) newcap <<= 1;
    seq_t *p = mem_resize(seq_t, A273670_cache, (size_t)newcap);
    if (!p) abort();
    A273670_cache = p;
    A273670_cap = newcap;
}

static seq_t A273670_a(idx_t n)
{
    if (!A273670_cache) {
        A273670_grow(32);
        A273670_cache[0] = (seq_t)1;
        A273670_csz = 1;
    }
    while (A273670_csz <= n) {
        seq_t cand = A273670_cache[A273670_csz - 1] + 1;
        while (!A273670_has_max(cand)) cand = cand + 1;
        A273670_grow(A273670_csz + 1);
        A273670_cache[A273670_csz++] = cand;
    }
    return A273670_cache[n];
}

static bool A256450_has_one(seq_t m)
{
    if (m == (seq_t)0) return false;
    seq_t t = m;
    idx_t i = 1;
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)(i + 1);
        if (d == (seq_t)1) return true;
        t = t / (seq_t)(i + 1);
        ++i;
    }
    return false;
}

static seq_t *A256450_cache = NULL;
static size_t A256450_cap = 0;
static idx_t A256450_len = 0;
static seq_t A256450_next = 2;

static seq_t A256450_a(idx_t n)
{
    if (!A256450_cache) {
        A256450_cap = 64;
        A256450_cache = mem_new(seq_t, A256450_cap);
        if (!A256450_cache) abort();
        A256450_cache[0] = (seq_t)1;
        A256450_len = 1;
    }
    while (A256450_len <= n) {
        while (!A256450_has_one(A256450_next)) ++A256450_next;
        if ((size_t)A256450_len >= A256450_cap) {
            A256450_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, A256450_cache, A256450_cap);
            if (!tmp) abort();
            A256450_cache = tmp;
        }
        A256450_cache[A256450_len++] = A256450_next;
        ++A256450_next;
    }
    return A256450_cache[n];
}

seq_t a(idx_t n)
{
    return A273670_a(n) - A256450_a(n);
}
