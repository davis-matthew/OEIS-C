/*
 * A297776  Number of distinct runs in base-8 digits of n.
 * https://oeis.org/A297776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct (digit,length) runs in the base‑8
 *            representation of n. For n ≥ 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)               /* sequence is defined only for n ≥ 1 */
        return 0;

    /* Extract base‑8 digits (least significant first).  A 64‑bit integer
       has at most 21 base‑8 digits, so 64 is an ample safety bound. */
    unsigned char digits[64];
    idx_t dcnt = 0;
    idx_t tmp  = n;
    while (tmp > 0) {
        digits[dcnt++] = tmp & 7;   /* tmp % 8  */
        tmp >>= 3;                  /* tmp /= 8 */
    }

    /* Scan the digits from most‑significant to least‑significant.
       `cur_digit` and `cur_len` hold the current run. */
    unsigned cur_digit = digits[dcnt - 1];
    idx_t    cur_len    = 1;

    struct pair {
        unsigned digit;
        idx_t    len;
    } uniques[64];          /* at most 64 distinct runs */
    idx_t uniq_cnt = 0;

    /* Process intermediate runs. */
    int i;
    for (i = (int)dcnt - 2; i >= 0; --i) {
        unsigned dig = digits[i];
        if (dig == cur_digit) {
            ++cur_len;
        } else {
            /* End of the current run: record it if unseen. */
            bool seen = false;
            for (idx_t j = 0; j < uniq_cnt; ++j) {
                if (uniques[j].digit == cur_digit &&
                    uniques[j].len   == cur_len) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                uniques[uniq_cnt].digit = cur_digit;
                uniques[uniq_cnt].len   = cur_len;
                ++uniq_cnt;
            }
            /* Begin a new run. */
            cur_digit = dig;
            cur_len   = 1;
        }
    }

    /* Handle the final (least‑significant) run. */
    bool seen_final = false;
    for (idx_t j = 0; j < uniq_cnt; ++j) {
        if (uniques[j].digit == cur_digit &&
            uniques[j].len   == cur_len) {
            seen_final = true;
            break;
        }
    }
    if (!seen_final) {
        uniques[uniq_cnt].digit = cur_digit;
        uniques[uniq_cnt].len   = cur_len;
        ++uniq_cnt;
    }

    return (seq_t)uniq_cnt;
}
