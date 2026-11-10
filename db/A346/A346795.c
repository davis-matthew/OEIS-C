/*
 * A346795  Irregular triangle T(n, k), n > 0, k = 1..A091220(n), read by
 *          rows; the n-th row gives, in ascending order, the distinct
 *          integers k such that A048720(k, m) = n for some m.
 * https://oeis.org/A346795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   flattened irregular triangle; row m lists, ascending, the
 *            integers d with the GF(2)[X] polynomial d dividing the
 *            polynomial m; row m has A091220(m) entries. Computed by direct
 *            divisor enumeration with cached row offsets
 * Range:     u8 n <= 1801, u16 n <= 9228, u32 n <= 9228, u64 n <= 9228,
 *            u128 n <= 9228, big any n
 * Validated: standard, pass, 97 terms through n = 7563 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int t_deg(uint64_t x)
{
    int r = -1;
    while (x) { x >>= 1; ++r; }
    return r;
}

static bool t_divides(uint64_t d, uint64_t m)
{
    int dd = t_deg(d);
    int dr = t_deg(m);
    while (m && dr >= dd) {
        m ^= d << (dr - dd);
        dr = t_deg(m);
    }
    return m == 0;
}

static uint64_t *t_cum = NULL;   /* t_cum[m] = number of entries in rows 1..m */
static uint64_t t_rows = 0;
static uint64_t t_cap = 0;

static void t_extend(void)
{
    if (t_rows + 1 >= t_cap) {
        t_cap = t_cap ? t_cap * 2 : 1024;
        t_cum = (uint64_t *)realloc(t_cum, t_cap * sizeof(uint64_t));
        if (t_rows == 0) t_cum[0] = 0;
    }
    uint64_t m = t_rows + 1;
    uint64_t cnt = 0;
    for (uint64_t d = 1; d <= m; ++d)
        if (t_divides(d, m)) ++cnt;
    t_cum[m] = t_cum[m - 1] + cnt;
    t_rows = m;
}

seq_t a(idx_t n)
{
    uint64_t idx = (uint64_t)n;
    if (idx == 0) return 0;
    if (t_cum == NULL) {
        t_cap = 1024;
        t_cum = (uint64_t *)malloc(t_cap * sizeof(uint64_t));
        t_cum[0] = 0;
        t_rows = 0;
    }
    while (t_cum[t_rows] < idx) t_extend();

    /* binary search for smallest row m with t_cum[m] >= idx */
    uint64_t lo = 1, hi = t_rows;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (t_cum[mid] >= idx) hi = mid;
        else lo = mid + 1;
    }
    uint64_t m = lo;
    uint64_t k = idx - t_cum[m - 1];   /* 1-based position in row m */
    uint64_t c = 0;
    for (uint64_t d = 1; d <= m; ++d) {
        if (t_divides(d, m)) {
            if (++c == k) return (seq_t)d;
        }
    }
    return 0;
}
