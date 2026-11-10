/*
 * A385748  Numbers k such that A384247(k) divides k.
 * https://oeis.org/A385748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) with A384247(k) | k, where A384247(k)
 *            = Prod_{p^e || k} (p^e - p^(e-2^v2(e))). Computed by a search
 *            over k = 2^a*3^b*5^c*17^d*257^e*65537^f < 2^64 (2 and the Fermat
 *            primes), sorted
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 206, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Validated: standard, pass, 63 terms through n = 250 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static const uint64_t A385748_P[6] = {2, 3, 5, 17, 257, 65537};
static uint64_t *A385748_terms = NULL;
static size_t A385748_cnt = 0, A385748_cap = 0;
static bool A385748_done = false;

static void A385748_push(uint64_t v)
{
    if (A385748_cnt == A385748_cap) {
        A385748_cap = A385748_cap ? A385748_cap * 2 : 1024;
        A385748_terms = (uint64_t *)realloc(A385748_terms, A385748_cap * sizeof(uint64_t));
    }
    A385748_terms[A385748_cnt++] = v;
}

static uint64_t A385748_pw(uint64_t p, unsigned e)
{
    uint64_t r = 1;
    while (e--) r *= p;
    return r;
}

static void A385748_rec(int i, uint64_t v, uint64_t A)
{
    if (i == 6) {
        if (v % A == 0) A385748_push(v);
        return;
    }
    uint64_t p = A385748_P[i];
    const uint64_t MAXV = ~(uint64_t)0;
    uint64_t cur = 1;
    for (unsigned e = 0;; ++e) {
        uint64_t f = 1;
        if (e > 0) {
            unsigned t = 1;
            unsigned x = e;
            while ((x & 1u) == 0) { x >>= 1; t <<= 1; }
            f = cur - A385748_pw(p, e - t);
        }
        A385748_rec(i + 1, v * cur, A * f);
        if (cur > MAXV / p) break;
        if (v > MAXV / (cur * p)) break;
        cur *= p;
    }
}

static int A385748_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    if (!A385748_done) {
        A385748_rec(0, 1, 1);
        qsort(A385748_terms, A385748_cnt, sizeof(uint64_t), A385748_cmp);
        A385748_done = true;
    }
    if (n == 0 || n > (idx_t)A385748_cnt) return (seq_t)0;
    return (seq_t)A385748_terms[n - 1];
}
