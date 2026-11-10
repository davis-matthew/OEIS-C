/*
 * A389141  Least positive integer k such that A389140(k*n) has the smallest
 *          value.
 * https://oeis.org/A389141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k>=1 with A389140(k*n) = min_j A389140(j*n); found
 *            by search over k, stopping at the conjectured minimum (1 if 5∤n,
 *            5 if 5|n and 25∤n, 15 if 25|n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9981 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *h389141_keys = NULL;
static uint64_t *h389141_vals = NULL;
static size_t h389141_cap = 0;
static size_t h389141_cnt = 0;

static size_t h389141_slot(uint64_t key, size_t cap)
{
    uint64_t h = key * 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)(h & (cap - 1));
}

static void h389141_grow(void)
{
    size_t ncap = h389141_cap ? h389141_cap * 2 : 1024;
    uint64_t *nk = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    uint64_t *nv = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    if (!nk || !nv) abort();
    for (size_t i = 0; i < h389141_cap; i++) {
        if (h389141_keys[i]) {
            size_t s = h389141_slot(h389141_keys[i], ncap);
            while (nk[s]) s = (s + 1) & (ncap - 1);
            nk[s] = h389141_keys[i];
            nv[s] = h389141_vals[i];
        }
    }
    free(h389141_keys);
    free(h389141_vals);
    h389141_keys = nk;
    h389141_vals = nv;
    h389141_cap = ncap;
}

static uint64_t h389141_get(uint64_t key)
{
    if (!h389141_cap) return 0;
    size_t s = h389141_slot(key, h389141_cap);
    while (h389141_keys[s]) {
        if (h389141_keys[s] == key) return h389141_vals[s];
        s = (s + 1) & (h389141_cap - 1);
    }
    return 0;
}

static void h389141_put(uint64_t key, uint64_t val)
{
    if ((h389141_cnt + 1) * 2 > h389141_cap) h389141_grow();
    size_t s = h389141_slot(key, h389141_cap);
    while (h389141_keys[s]) {
        if (h389141_keys[s] == key) { h389141_vals[s] = val; return; }
        s = (s + 1) & (h389141_cap - 1);
    }
    h389141_keys[s] = key;
    h389141_vals[s] = val;
    h389141_cnt++;
}

static uint64_t A389140_min_terminal(uint64_t x)
{
    uint64_t c = h389141_get(x);
    if (c) return c;

    uint8_t digits[32];
    unsigned len = 0;
    {
        uint8_t rev[32];
        uint64_t t = x;
        while (t > 0) { rev[len++] = (uint8_t)(t % 10); t /= 10; }
        for (unsigned i = 0; i < len; i++) digits[i] = rev[len - 1 - i];
    }
    bool has_even = false;
    for (unsigned i = 0; i < len; i++)
        if ((digits[i] & 1u) == 0) { has_even = true; break; }
    if (!has_even) { h389141_put(x, x); return x; }

    uint64_t best = x;
    for (unsigned start = 0; start < len && best != 1; start++) {
        if (digits[start] == 0) continue;
        uint64_t sub = 0;
        for (unsigned end = start; end < len; end++) {
            sub = sub * 10 + digits[end];
            if (digits[end] & 1u) continue;
            uint64_t q = sub >> 1;
            uint64_t nn = 0;
            for (unsigned i = 0; i < start; i++) nn = nn * 10 + digits[i];
            uint8_t rq[32];
            unsigned qc = 0;
            uint64_t tq = q;
            while (tq > 0) { rq[qc++] = (uint8_t)(tq % 10); tq /= 10; }
            if (qc == 0) rq[qc++] = 0;
            for (int i = (int)qc - 1; i >= 0; i--) nn = nn * 10 + rq[i];
            for (unsigned i = end + 1; i < len; i++) nn = nn * 10 + digits[i];
            uint64_t cand = A389140_min_terminal(nn);
            if (cand < best) {
                best = cand;
                if (best == 1) break;
            }
        }
    }
    h389141_put(x, best);
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t target;
    if (n % 5 != 0) target = 1;
    else if (n % 25 != 0) target = 5;
    else target = 15;

    uint64_t best = 0, bestk = 1;
    for (uint64_t k = 1; k <= 20000000ULL; k++) {
        uint64_t v = A389140_min_terminal((uint64_t)n * k);
        if (v == target) return (seq_t)k;
        if (best == 0 || v < best) { best = v; bestk = k; }
    }
    return (seq_t)bestk;
}
