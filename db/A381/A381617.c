/*
 * A381617  A sequence constructed by greedily sampling the zeta distribution
 *          for parameter value 2 to minimize discrepancy.
 * https://oeis.org/A381617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greedily minimize discrepancy of the zeta(2) distribution
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external types supplied by the build system */
extern unsigned long __INT128;

seq_t a(idx_t n)
{
    /* Constant zeta(2) = π²/6  (long double for extra precision) */
    const long double zeta2 = 1.6449340668482264365L;

    /* Static state for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;        /* number of computed terms */
    static size_t seq_cap = 0;        /* allocated capacity of seq array */

    static uint32_t *cnt = NULL;
    static size_t cnt_cap = 0;        /* allocated capacity of counts array */

    if (n == 0)   /* not used; offset is 1 */
        return 0;

    /* Ensure capacity for sequence array */
    if (seq_len < (size_t)n) {
        size_t new_cap = n * 2;
        if (new_cap < 64) new_cap = 64;
        seq_t *tmp_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp_seq) exit(EXIT_FAILURE);
        seq = tmp_seq;
        seq_cap = new_cap;
    }

    /* Ensure capacity for counts array */
    if (cnt_cap < (size_t)(n + 2)) {
        size_t new_cap = (cnt_cap + 1) * 2;
        if (new_cap < (size_t)(n + 2)) new_cap = (size_t)(n + 2);
        uint32_t *tmp_cnt = (uint32_t *)realloc(cnt, new_cap * sizeof(uint32_t));
        if (!tmp_cnt) exit(EXIT_FAILURE);
        for (size_t i = cnt_cap; i < new_cap; ++i) tmp_cnt[i] = 0;
        cnt = tmp_cnt;
        cnt_cap = new_cap;
    }

    /* Generate terms up to n */
    for (idx_t cur = (idx_t)seq_len + 1; cur <= n; ++cur) {
        long double best_disc = -1e300L;
        idx_t best_k = 1;

        /* Since expected number of distinct values ≤ cur, testing k up to cur
           is safe and fast enough for n ≤ 10 000 (≈50 M iterations).   */
        for (idx_t k = 1; k <= cur; ++k) {
            long double exp_cnt = (long double)cur / (zeta2 * (long double)k * (long double)k);
            long double disc = exp_cnt - (long double)cnt[k];
            if (disc > best_disc) {
                best_disc = disc;
                best_k = k;
            }
        }

        seq[cur - 1] = (seq_t)best_k;
        ++cnt[best_k];
    }

    seq_len = (size_t)n;
    return seq[n - 1];
}
