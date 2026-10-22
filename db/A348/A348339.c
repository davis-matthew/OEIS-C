/*
 * A348339  a(n) is the number of distinct numbers of steps required for the
 *          last n digits of integers to repeat themselves by iterating the
 *          map m -> m^2.
 * https://oeis.org/A348339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = a(n‑1) + 2*n – 3 – ceil(log_5((n‑1)/16)) for n ≥ 5
 *            Initial values: a(1)=3, a(2)=6, a(3)=9, a(4)=12
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   ceil_log5_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_log5_term.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1 */

    seq_t val = 0;         /* will hold a(i‑1) during the loop */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t cur;
        if (i == 1)
            cur = 3;
        else if (i == 2)
            cur = 6;
        else if (i == 3)
            cur = 9;
        else if (i == 4)
            cur = 12;
        else {
            seq_t term = ((seq_t)i * 2) - 3;   /* 2*i – 3 */
            term -= ceil_log5_term(i);        /* minus the ceil(log_5 ...) */
            cur = val + term;                 /* a(i‑1) + term */
        }
        val = cur;
    }
    return val;
}
