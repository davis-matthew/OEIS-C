/*
 * A276384  Defined by the properties that it starts with 0, and when you
 *          successively apply DIFF, RUNS, BISECT, RUNS you get (4,1,3,1)
 *          repeated infinitely often.
 * https://oeis.org/A276384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n differences of the infinite sequence
 *            of differences defined by 1, [5 or 6 of 2], 1, [5 or 6 of 2], 1,
 *            … The lengths of the runs of 2's follow the periodic pattern
 *            5,5,5,5,6,5,5,5,6,5,5,5,6,…, i.e. runs of 5 of length
 *            alternating 4 and 3 separated by single 6. Thus a(n) =...
 * Range:     u8 n <= 139, u16 n <= 9201, u32 n <= 9201, u64 n <= 9201,
 *            u128 n <= 9201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t val = 0;          /* current value of the sequence */
    idx_t steps_left = n;   /* how many differences still have to be applied */

    /* current run of the difference sequence */
    bool diff_is_one = true;        /* 1‑run or 2‑run */
    idx_t run_remain = 1;           /* how many steps are left in the current run */

    /* state for generating the 2‑run lengths (f‑sequence) */
    idx_t f5_len_current = 4;      /* current length of a block of 5’s (4 or 3) */
    idx_t f5_remaining = 4;        /* how many 5’s are left in this block */
    bool f_next_is_five = true;    /* next value of the f‑sequence (5 or 6) */

    while (steps_left > 0) {
        idx_t take = steps_left < run_remain ? steps_left : run_remain;

        if (diff_is_one) {
            val += (seq_t)take;                 /* add 1 for each step */
        } else {
            val += (seq_t)(take * 2);           /* add 2 for each step */
        }

        steps_left -= take;
        run_remain -= take;

        if (run_remain == 0) {                 /* current run finished */
            diff_is_one = !diff_is_one;        /* toggle between 1‑run and 2‑run */

            if (diff_is_one) {                 /* 1‑run always has length 1 */
                run_remain = 1;
            } else {                           /* start a 2‑run: fetch next f‑value */
                idx_t f_val;
                if (f_next_is_five) {          /* produce a 5 */
                    f_val = 5;
                    f5_remaining--;
                    if (f5_remaining == 0) {
                        f_next_is_five = false; /* next will be a 6 */
                    }
                } else {                       /* produce a 6 */
                    f_val = 6;
                    f_next_is_five = true;     /* next will be a 5 */
                    f5_len_current = (f5_len_current == 4) ? 3 : 4;
                    f5_remaining = f5_len_current;
                }
                run_remain = f_val;
            }
        }
    }

    return val;
}
