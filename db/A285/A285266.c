/*
 * A285266  Array read by antidiagonals: T(m,n) = number of m-ary words of
 *          length n with adjacent elements differing by 2 or less.
 * https://oeis.org/A285266
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) read along antidiagonals of the table defined by
 *            T(m,k) = # of m‑ary words of length k with adjacent symbols
 *            differing by at most 2. m ≥ 3, k ≥ 0. Off‑set is 3, so a(3) is
 *            the first listed term
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 193, u64 n <= 600,
 *            u128 n <= 1277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Count words of length len over an m‑ary alphabet where neighbouring
   symbols differ by at most 2.  Uses a two‑row dynamic program. */
static seq_t count_words(idx_t m, idx_t len)
{
    /* Empty word */
    if (len == 0) return (seq_t)1;
    /* Should never happen for valid arguments */
    if (m == 0) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation – std::vector handles construction and destruction. */
    std::vector<seq_t> dp_prev(m, seq_t(1));
    std::vector<seq_t> dp_cur(m);
    for (idx_t l = 2; l <= len; ++l) {
        for (idx_t x = 0; x < m; ++x) {
            seq_t sum = seq_t(0);
            if (x >= 2) sum += dp_prev[x-2];
            if (x >= 1) sum += dp_prev[x-1];
            sum += dp_prev[x];
            if (x + 1 < m) sum += dp_prev[x+1];
            if (x + 2 < m) sum += dp_prev[x+2];
            dp_cur[x] = sum;
        }
        dp_prev.swap(dp_cur);
    }
    seq_t total = seq_t(0);
    for (idx_t i = 0; i < m; ++i) total += dp_prev[i];
    return total;
#else
    /* C implementation – raw memory allocation; seq_t is a plain integer type. */
    seq_t *dp_prev = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)malloc((size_t)m * sizeof(seq_t));
    if (!dp_prev || !dp_cur) { free(dp_prev); free(dp_cur); return (seq_t)0; }

    for (idx_t i = 0; i < m; ++i) dp_prev[i] = (seq_t)1;
    for (idx_t l = 2; l <= len; ++l) {
        for (idx_t x = 0; x < m; ++x) {
            seq_t sum = 0;
            if (x >= 2) sum += dp_prev[x-2];
            if (x >= 1) sum += dp_prev[x-1];
            sum += dp_prev[x];
            if (x + 1 < m) sum += dp_prev[x+1];
            if (x + 2 < m) sum += dp_prev[x+2];
            dp_cur[x] = sum;
        }
        seq_t *tmp = dp_prev; dp_prev = dp_cur; dp_cur = tmp;
    }
    seq_t total = 0;
    for (idx_t i = 0; i < m; ++i) total += dp_prev[i];
    free(dp_prev);
    free(dp_cur);
    return total;
#endif
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;  /* undefined for indices below offset */

    idx_t idx = n - 3;  /* zero‑based ordinal in the antidiagonal sequence */

    /* Find the diagonal d such that (d+1)(d+2)/2 > idx.
       For n up to ~10000, 64‑bit arithmetic is sufficient. */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2)) / 2 <= idx ) ++d;

    idx_t cumulative_before = (d * (d + 1)) / 2; /* number of entries before this diagonal */
    idx_t pos = idx - cumulative_before;        /* position within diagonal */

    idx_t m   = 3 + pos;          /* alphabet size (m ≥ 3) */
    idx_t len = d - pos;          /* word length (len ≥ 0) */

    return count_words(m, len);
}
