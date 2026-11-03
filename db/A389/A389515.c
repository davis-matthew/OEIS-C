/*
 * A389515  Standard composition numbers of compositions whose maximal runs R
 *          all satisfy length(R) > first(R).
 * https://oeis.org/A389515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that the composition
 *            obtained from k in standard order has every maximal run R
 *            satisfy length(R) > first(R)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool comp_ok(idx_t k)
{
    idx_t last_pos = 0;      /* position of the last '1', or 0 for the prepended zero   */
    idx_t run_len = 0;       /* length of the current run of equal parts              */
    idx_t prev_part = 0;     /* value of the current run                             */
    bool first_run = true;   /* whether we are still before the first part            */
    bool ok = true;
    idx_t t = k;
    idx_t bit = 0;           /* position of the current bit (0‑based)                */

    while (t) {
        if (t & 1ULL) {                     /* found a 1‑bit at position 'bit'          */
            idx_t pos = bit + 1;             /* positions are 1‑based                     */
            idx_t part = pos - last_pos;     /* length of the next part                  */

            if (first_run) {                 /* start the first run                      */
                prev_part = part;
                run_len = 1;
                first_run = false;
            } else if (part == prev_part) {  /* continue the current run                 */
                run_len++;
            } else {                         /* new run starts                             */
                if (run_len <= prev_part) {  /* previous run violated condition            */
                    ok = false;
                    break;
                }
                prev_part = part;
                run_len = 1;
            }
            last_pos = pos;                   /* update last position of a 1‑bit          */
        }
        t >>= 1;          /* move to the next bit                     */
        bit++;
    }

    if (ok && !first_run) {                     /* check the final run                  */
        if (run_len <= prev_part) ok = false;
    }

    return ok;
}

seq_t a(idx_t n)
{
    idx_t k = 0;          /* current index in standard order          */
    idx_t count = 0;      /* how many qualifying compositions seen   */

    while (true) {
        if (comp_ok(k)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
