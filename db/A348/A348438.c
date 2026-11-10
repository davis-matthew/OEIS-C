/*
 * A348438  Where prime(n) appears for the first time in A088177.
 * https://oeis.org/A348438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A088177(k) = prime(n); computed by
 *            generating A088177 and searching
 * Range:     u8 n <= 9, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 73 terms through n = 86 (2026-11-10)
 * Helpers:   mem, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/nth_prime.h"

static inline size_t A348438_hash(seq_t x)
{
    uint64_t h = (uint64_t)x;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccdULL;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53ULL;
    h ^= h >> 33;
    return (size_t)h;
}

static seq_t *A348438_arr = NULL;
static size_t A348438_cap = 0;
static idx_t  A348438_len = 0;

static void A348438_ensure(size_t need)
{
    if (need <= A348438_cap) return;
    size_t newcap = A348438_cap ? A348438_cap * 2 : 8;
    while (newcap < need) newcap <<= 1;
    seq_t *p = mem_resize(seq_t, A348438_arr, newcap);
    if (!p) abort();
    A348438_arr = p;
    A348438_cap = newcap;
}

static seq_t *A348438_tab = NULL;
static size_t A348438_mask = 0;
static size_t A348438_cnt = 0;

static void A348438_tab_init(void)
{
    if (A348438_tab) return;
    A348438_mask = 15;
    A348438_tab = mem_new(seq_t, A348438_mask + 1);
    if (!A348438_tab) abort();
    A348438_cnt = 0;
}

static void A348438_insert(seq_t value)
{
    A348438_tab_init();
    if ((A348438_cnt + 1) * 10 > (A348438_mask + 1) * 7) {
        size_t old_cap = A348438_mask + 1;
        size_t new_cap = old_cap << 1;
        size_t new_mask = new_cap - 1;
        seq_t *nt = mem_new(seq_t, new_cap);
        if (!nt) abort();
        for (size_t i = 0; i < old_cap; ++i) {
            seq_t v = A348438_tab[i];
            if (!v) continue;
            size_t pos = A348438_hash(v) & new_mask;
            while (nt[pos]) pos = (pos + 1) & new_mask;
            nt[pos] = v;
        }
        mem_free(A348438_tab);
        A348438_tab = nt;
        A348438_mask = new_mask;
    }
    size_t pos = A348438_hash(value) & A348438_mask;
    while (A348438_tab[pos]) pos = (pos + 1) & A348438_mask;
    A348438_tab[pos] = value;
    ++A348438_cnt;
}

static bool A348438_contains(seq_t value)
{
    if (!A348438_tab) return false;
    size_t pos = A348438_hash(value) & A348438_mask;
    while (A348438_tab[pos]) {
        if (A348438_tab[pos] == value) return true;
        pos = (pos + 1) & A348438_mask;
    }
    return false;
}

static seq_t A088177_a(idx_t n)
{
    if (n == 0) return 0;
    if (!A348438_arr) {
        A348438_ensure(8);
        A348438_arr[0] = (seq_t)1;
        A348438_arr[1] = (seq_t)1;
        A348438_len = 2;
        A348438_tab_init();
        A348438_insert((seq_t)1);
    }
    for (idx_t idx = A348438_len + 1; idx <= n; ++idx) {
        seq_t prev = A348438_arr[idx - 2];
        seq_t cur = (seq_t)1;
        seq_t product;
        while (true) {
            product = prev * cur;
            if (!A348438_contains(product)) break;
            ++cur;
        }
        A348438_ensure((size_t)A348438_len + 1);
        A348438_arr[A348438_len] = cur;
        ++A348438_len;
        A348438_insert(product);
    }
    return A348438_arr[n - 1];
}

static idx_t A348438_scanned = 0;

seq_t a(idx_t n)
{
    seq_t p = (seq_t)nth_prime(n);
    /* search already generated terms first */
    for (idx_t k = 1; k <= A348438_len; ++k) {
        if (A348438_arr[k - 1] == p) return (seq_t)k;
    }
    (void)A348438_scanned;
    for (idx_t k = A348438_len + 1;; ++k) {
        if (A088177_a(k) == p) return (seq_t)k;
    }
}
