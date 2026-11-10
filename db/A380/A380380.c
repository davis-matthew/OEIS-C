/*
 * A380380  Numbers k such that A380377(k)/k (proportion of supporters needed
 *          to win an election when there are k voters) sets a new minimum.
 * https://oeis.org/A380380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th k at which A380377(k)/k reaches a new *
 *            minimum. Here A380377(k) = min over d of the votes needed to win
 *            a majority * of d balanced districts. * To find each k, the
 *            search checks only those d where the lower bound * needed(k,d)
 *            >= k/4 + k/(4d) + d/8 * can still beat the c...
 * Range:     u8 n <= 37, u16 n <= 1788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9110 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static idx_t A380380_needed(idx_t n, idx_t d)
{
    idx_t q = n / d;
    idx_t r = n - q * d;
    idx_t small_cnt = d - r;
    idx_t t_small = (q / 2) + 1;
    idx_t t_large = ((q + 1) / 2) + 1;
    idx_t win = (d / 2) + 1;
    if (small_cnt >= win) return win * t_small;
    return small_cnt * t_small + (win - small_cnt) * t_large;
}

static idx_t A380377_a(idx_t n)
{
    idx_t best = n + 1;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t v = A380380_needed(n, d);
        if (v < best) best = v;
    }
    return best;
}

static idx_t *A380380_recs = 0;
static size_t A380380_cnt = 0, A380380_cap = 0;
static idx_t A380380_scanned = 0;
static idx_t A380380_bv = 0, A380380_bk = 0;

static void A380380_push(idx_t k, idx_t v)
{
    if (A380380_cnt == A380380_cap) {
        A380380_cap = A380380_cap ? A380380_cap * 2 : 256;
        A380380_recs = (idx_t *)realloc(A380380_recs, A380380_cap * sizeof(idx_t));
    }
    A380380_recs[A380380_cnt++] = k;
    A380380_bv = v;
    A380380_bk = k;
}

seq_t a(idx_t n)
{
    while (A380380_cnt < (size_t)n) {
        idx_t k = ++A380380_scanned;
        if (A380380_cnt == 0) {
            A380380_push(k, A380377_a(k));
            continue;
        }
        idx_t bv = A380380_bv, bk = A380380_bk;
        /* need d with 2k/d + d < M, M = 8*T - 2k, T = bv*k/bk */
        double M = 8.0 * (double)bv * (double)k / (double)bk - 2.0 * (double)k;
        if (M < -1.0) continue;
        double disc = M * M - 8.0 * (double)k;
        double sq = disc > 0.0 ? sqrt(disc) : 0.0;
        double lo_d = (M - sq) / 2.0 - 2.0;
        double hi_d = (M + sq) / 2.0 + 2.0;
        if (hi_d < 1.0) continue;
        idx_t lo = lo_d < 1.0 ? 1 : (idx_t)lo_d;
        idx_t hi = hi_d > (double)k ? k : (idx_t)hi_d;
        if (lo < 1) lo = 1;
        idx_t best = 0;
        int found = 0;
        for (idx_t d = lo; d <= hi; ++d) {
            idx_t v = A380380_needed(k, d);
            if (v * bk < bv * k) {
                if (!found || v < best) best = v;
                found = 1;
            }
        }
        if (found) A380380_push(k, best);
    }
    return (seq_t)A380380_recs[n - 1];
}
