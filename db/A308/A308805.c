/*
 * A308805  The image of N X N under f, where f(x,y) = 2*x*y*(x*y-1)-x+y.
 * https://oeis.org/A308805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for each n, enumerate integer pairs (x,y) with xy = n sorted by x descending, compute f(x,y) = 2*x*y*(x*y-1)-x+y. */
#include <stdlib.h>

static size_t divisor_count(idx_t m)
{
    size_t cnt = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            if (i * i == m) cnt += 1;
            else cnt += 2;
        }
    }
    return cnt;
}

static int cmp_desc(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset is 1 */

    static idx_t *cum = NULL;
    static size_t cum_size = 0;
    static idx_t *divs = NULL;
    static size_t div_cap = 0;

    /* extend cumulative array until it covers index n */
    while (cum_size == 0 || cum[cum_size - 1] < n) {
        idx_t row = cum_size + 1;
        size_t d = divisor_count(row);
        idx_t new_cum = (cum_size == 0 ? d : cum[cum_size - 1] + d);
        cum_size++;
        cum = (idx_t *)realloc(cum, cum_size * sizeof(idx_t));
        cum[cum_size - 1] = new_cum;

        if (d > div_cap) {
            div_cap = d;
            divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
        }
    }

    /* locate row r where the term lies */
    size_t r = cum_size;               /* last row added, 1‑based */
    idx_t prev_cum = (r > 1) ? cum[r - 2] : 0;
    size_t pos = (size_t)(n - prev_cum - 1); /* 0‑based */

    /* gather divisors of r */
    size_t d = divisor_count(r);
    size_t idx = 0;
    for (idx_t i = 1; idx < d && i * i <= r; ++i) {
        if (r % i == 0) {
            divs[idx++] = i;
            if (i * i != r) {
                divs[idx++] = r / i;
            }
        }
    }

    /* sort divisors descending */
    qsort(divs, d, sizeof(idx_t), cmp_desc);

    idx_t x = divs[pos];
    idx_t y = r / x;

    /* compute f(x,y) */
    seq_t xy = (seq_t)x * (seq_t)y;
    seq_t term = 2 * xy * (xy - 1) - x + y;
    return term;
}
