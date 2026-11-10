/*
 * A348357  Square array T(n, k), n, k > 0, read by antidiagonal upwards; the
 *          k-th column contains, in ascending order, the integers m such that
 *          A348331(m) = k.
 * https://oeis.org/A348357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   search/simulation. A348331 is built greedily (a(i) = least v
 *            with sum of earlier indices of v <= i); T(n,k) = n-th index m
 *            with A348331(m)=k; flat index N -> antidiagonal d, k = N -
 *            d(d-1)/2, n = d-k+1
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *t_vals = NULL;   /* t_vals[i] = A348331(i) */
static uint64_t *t_sum = NULL;    /* t_sum[v] = sum of indices where value v used */
static uint64_t t_len = 0, t_cap = 0, t_scap = 0;

static void t_extend(uint64_t newlen)
{
    if (newlen + 1 > t_cap) {
        t_cap = newlen + 1;
        t_vals = (uint64_t *)realloc(t_vals, t_cap * sizeof(uint64_t));
    }
    for (uint64_t i = t_len + 1; i <= newlen; ++i) {
        for (uint64_t v = 1;; ++v) {
            if (v >= t_scap) {
                uint64_t nc = t_scap ? t_scap * 2 : 64;
                while (nc <= v) nc *= 2;
                t_sum = (uint64_t *)realloc(t_sum, nc * sizeof(uint64_t));
                for (uint64_t q = t_scap; q < nc; ++q) t_sum[q] = 0;
                t_scap = nc;
            }
            if (t_sum[v] <= i) {
                t_vals[i] = v;
                t_sum[v] += i;
                break;
            }
        }
        t_len = i;
    }
}

seq_t a(idx_t N)
{
    if (N < 1) return (seq_t)0;
    uint64_t d = 1;
    while ((uint64_t)d * (d + 1) / 2 < (uint64_t)N) ++d;
    uint64_t k = (uint64_t)N - d * (d - 1) / 2;
    uint64_t n = d - k + 1;

    uint64_t target = t_len < 64 ? 64 : t_len;
    for (;;) {
        if (target > t_len) t_extend(target);
        uint64_t c = 0;
        for (uint64_t i = 1; i <= t_len; ++i) {
            if (t_vals[i] == k && ++c == n) return (seq_t)i;
        }
        target = t_len * 2;
    }
}
