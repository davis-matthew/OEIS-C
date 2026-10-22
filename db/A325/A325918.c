/*
 * A325918  Number of Motzkin excursions of length n with an even number of
 *          humps and without peaks.
 * https://oeis.org/A325918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   DP for Motzkin excursions of length n with an even number of
 *            humps and no peaks. Initial values: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=1, a(5)=1, …
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Trivial case: length 0 – only the horizontal path HH…H. */
    if (n == 0) return (seq_t)1;

    const size_t max_h = (size_t)n;   /* highest possible altitude */
    const size_t stride = 6;          /* 3 states × 2 parity values */

    seq_t *cur  = (seq_t *)calloc((max_h + 1) * stride, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((max_h + 1) * stride, sizeof(seq_t));
    if (!cur || !next) return (seq_t)0;   /* allocation failure */

    /* Start at altitude 0, normal state (0), even parity (0). */
    cur[0 * stride + 0 * 2 + 0] = (seq_t)1;

    for (idx_t pos = 0; pos < n; ++pos) {
        /* Clear the buffer for the next step. */
        memset(next, 0, (max_h + 1) * stride * sizeof(seq_t));

        for (size_t h = 0; h <= (size_t)pos; ++h) {
            const size_t base = h * stride;
            for (int state = 0; state < 3; ++state) {
                for (int p = 0; p < 2; ++p) {
                    seq_t val = cur[base + state * 2 + p];
                    if (!val) continue;

                    /* Horizontal step (H). */
                    int hstate = (state == 0) ? 0 : 2;  /* S0→S0, S1→S2, S2→S2 */
                    next[base + hstate * 2 + p] += val;

                    /* Up step (U). */
                    next[(h + 1) * stride + 1 * 2 + p] += val;

                    /* Down step (D). */
                    if (h == 0) continue;              /* cannot go below ground */
                    if (state == 1) continue;          /* would create a peak */

                    int new_p = p;                     /* parity unchanged in normal case */
                    if (state == 2) new_p ^= 1;        /* closing a hump flips parity */

                    next[(h - 1) * stride + 0 * 2 + new_p] += val;
                }
            }
        }

        /* Swap current and next buffers. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Result: altitude 0, normal state, even parity. */
    return cur[0 * stride + 0 * 2 + 0];
}
