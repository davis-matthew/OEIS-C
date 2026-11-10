/*
 * A259229  Inverse permutation to A259228.
 * https://oeis.org/A259229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A259228(k) = n. Computed by generating
 *            A259228 in order (for each k, take the shortest prefix of the
 *            significant digits of 1/k that has not been used yet) and
 *            recording where each value first appears
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9755 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A259229_keys = NULL;
static uint64_t *A259229_vals = NULL;
static size_t A259229_cap = 0, A259229_cnt = 0;
static uint64_t A259229_k = 0;

static size_t A259229_hash(uint64_t x, size_t cap)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    return (size_t)(x & (uint64_t)(cap - 1));
}

static void A259229_insert_raw(uint64_t m, uint64_t v)
{
    size_t h = A259229_hash(m, A259229_cap);
    while (A259229_keys[h] != 0) h = (h + 1) & (A259229_cap - 1);
    A259229_keys[h] = m;
    A259229_vals[h] = v;
}

static void A259229_insert(uint64_t m, uint64_t v)
{
    if (A259229_cap == 0 || (A259229_cnt + 1) * 2 > A259229_cap) {
        size_t nc = A259229_cap ? A259229_cap * 2 : 1024;
        uint64_t *ok = A259229_keys, *ov = A259229_vals;
        size_t oc = A259229_cap;
        A259229_keys = (uint64_t *)calloc(nc, sizeof(uint64_t));
        A259229_vals = (uint64_t *)calloc(nc, sizeof(uint64_t));
        if (!A259229_keys || !A259229_vals) exit(1);
        A259229_cap = nc;
        for (size_t i = 0; i < oc; i++)
            if (ok[i] != 0) A259229_insert_raw(ok[i], ov[i]);
        free(ok);
        free(ov);
    }
    A259229_insert_raw(m, v);
    A259229_cnt++;
}

/* returns stored index or 0 if absent */
static uint64_t A259229_find(uint64_t m)
{
    if (A259229_cap == 0) return 0;
    size_t h = A259229_hash(m, A259229_cap);
    while (A259229_keys[h] != 0) {
        if (A259229_keys[h] == m) return A259229_vals[h];
        h = (h + 1) & (A259229_cap - 1);
    }
    return 0;
}

/* compute A259228(k) for the next k and record it */
static void A259229_step(void)
{
    uint64_t k = ++A259229_k;
    if (k == 1) {
        A259229_insert(1, 1);
        return;
    }
    uint64_t rem = 1, cand = 0;
    bool started = false;
    for (;;) {
        rem *= 10;
        uint64_t d = rem / k;
        rem %= k;
        if (!started) {
            if (d == 0) continue;
            started = true;
        }
        if (cand > (UINT64_MAX - 9) / 10) return; /* unreachable in practice */
        cand = cand * 10 + d;
        if (A259229_find(cand) == 0) {
            A259229_insert(cand, k);
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t v;
    while ((v = A259229_find((uint64_t)n)) == 0)
        A259229_step();
    return (seq_t)v;
}
