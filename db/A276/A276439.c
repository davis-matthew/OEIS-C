/*
 * A276439  Partial sums of A276438.
 * https://oeis.org/A276439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(n) = A276438(n) + a(n-1), where A276438(n) =
 *            (A274647(n)-A274647(n-1))/n is found by a greedy search
 * Range:     u8 n <= 46, u16 n <= 9360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9847 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *ps_cache = NULL;
static size_t ps_cap = 0;
static idx_t ps_last = 0;
static uint8_t *ps_used = NULL;
static size_t ps_used_bytes = 0;
static seq_t ps_cur = 0;

static void ps_used_set(seq_t v)
{
    size_t idx = (size_t)(unsigned long long)v;
    size_t byte = idx >> 3;
    if (byte >= ps_used_bytes) {
        size_t nb = byte + 1 + byte / 2;
        uint8_t *tmp = (uint8_t *)realloc(ps_used, nb);
        memset(tmp + ps_used_bytes, 0, nb - ps_used_bytes);
        ps_used = tmp;
        ps_used_bytes = nb;
    }
    ps_used[byte] |= (uint8_t)(1U << (idx & 7));
}

static int ps_used_has(seq_t v)
{
    size_t idx = (size_t)(unsigned long long)v;
    size_t byte = idx >> 3;
    if (byte >= ps_used_bytes) return 0;
    return (ps_used[byte] >> (idx & 7)) & 1;
}

seq_t a(idx_t n)
{
    if (ps_cache == NULL) {
        ps_cap = 64;
        ps_cache = mem_new(seq_t, ps_cap);
        ps_cache[0] = 0;
        ps_used_set(0);
    }
    if (n <= ps_last) return ps_cache[n];
    if ((size_t)n + 1 > ps_cap) {
        size_t nc = ps_cap;
        while (nc < (size_t)n + 1) nc <<= 1;
        ps_cache = mem_resize(seq_t, ps_cache, nc);
        ps_cap = nc;
    }
    for (idx_t i = ps_last + 1; i <= n; ++i) {
        seq_t inc = (seq_t)i;
        seq_t chosen_val = 0, chosen_k = 0;
        int sign = 0;
        for (seq_t k = 1; ; ++k) {
            seq_t prod = inc * k;
            seq_t plus = ps_cur + prod;
            int plus_ok = !ps_used_has(plus);
            int minus_ok = 0;
            seq_t minus = 0;
            if (ps_cur >= prod) {
                minus = ps_cur - prod;
                minus_ok = !ps_used_has(minus);
            }
            if (plus_ok || minus_ok) {
                if (plus_ok && minus_ok) {
                    if (minus <= plus) { chosen_val = minus; sign = -1; }
                    else { chosen_val = plus; sign = 1; }
                } else if (plus_ok) { chosen_val = plus; sign = 1; }
                else { chosen_val = minus; sign = -1; }
                chosen_k = k;
                break;
            }
        }
        if (sign > 0) ps_cache[i] = ps_cache[i - 1] + chosen_k;
        else ps_cache[i] = ps_cache[i - 1] - chosen_k;
        ps_cur = chosen_val;
        ps_used_set(chosen_val);
        ps_last = i;
    }
    return ps_cache[n];
}
