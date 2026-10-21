/*
 * A247748  Total area below all lattice paths from (0,0) to (n,0) that do not
 *          go below the x-axis or above the diagonal x=y and consist of steps
 *          u=(1,1), U=(1,3), H=(1,0), d=(1,-1) and D=(1,-3).
 * https://oeis.org/A247748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total area below all lattice paths from (0,0) to (n,0)
 *            that * stay between y = 0 and y = x, using steps (1,1)=u,
 *            (1,3)=U, (1,0)=H, * (1,-1)=d, (1,-3)=D. Computed by dynamic
 *            programming over step number and * current height
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* arrays sized to accommodate heights up to n */
    idx_t sz = n + 2;
    seq_t *cnt = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *area = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *cnt_next = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *area_next = (seq_t*)calloc(sz, sizeof(seq_t));

    cnt[0] = (seq_t)1;  /* one empty path at height 0 */

    for (idx_t step = 0; step < n; ++step) {
        /* clear next arrays */
        memset(cnt_next, 0, sz * sizeof(seq_t));
        memset(area_next, 0, sz * sizeof(seq_t));

        idx_t max_y = step;  /* reachable heights after 'step' steps */

        for (idx_t y = 0; y <= max_y; ++y) {
            seq_t c = cnt[y];
            if (c == (seq_t)0) continue;
            seq_t a_start = area[y];
            seq_t y_seq = (seq_t)y;
            seq_t add_area = a_start + c * y_seq; /* area added for this step */

            /* u = +1 */
            idx_t y1 = y + 1;
            if (y1 <= step + 1) {
                cnt_next[y1] += c;
                area_next[y1] += add_area;
            }

            /* U = +3 */
            y1 = y + 3;
            if (y1 <= step + 1) {
                cnt_next[y1] += c;
                area_next[y1] += add_area;
            }

            /* H = 0 */
            y1 = y;
            cnt_next[y1] += c;
            area_next[y1] += add_area;

            /* d = -1 */
            if (y >= 1) {
                y1 = y - 1;
                cnt_next[y1] += c;
                area_next[y1] += add_area;
            }

            /* D = -3 */
            if (y >= 3) {
                y1 = y - 3;
                cnt_next[y1] += c;
                area_next[y1] += add_area;
            }
        }

        /* swap current and next arrays for next iteration */
        seq_t *tmp;
        tmp = cnt;  cnt = cnt_next;  cnt_next = tmp;
        tmp = area; area = area_next; area_next = tmp;
    }

    seq_t result = area[0]; /* only paths ending at height 0 count */

    /* free allocated memory */
    free(cnt);
    free(area);
    free(cnt_next);
    free(area_next);

    return result;
}
