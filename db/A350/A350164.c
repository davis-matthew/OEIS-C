/*
 * A350164  a(n) = Sum_{k=1..n}(-1)^(k+1) * floor(n/(2*k-1))^n.
 * https://oeis.org/A350164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t max_k = (n + 1) / 2;          /* largest k with 2*k-1 <= n */
    int   cur_coef = 0;                  /* coefficient for current d (±1 or 0) */
    idx_t last_d   = 0;                  /* last distinct d value */
    bool  has_last = false;              /* have we started a group yet? */

    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t denom = 2 * k - 1;
        idx_t d = n / denom;             /* floor division */

        /* If d has changed, flush the previous group */
        if (has_last && d != last_d) {
            if (cur_coef != 0) {
                seq_t pow_val = big_pow((seq_t)last_d, (seq_t)n);
                if (cur_coef > 0)
                    result += pow_val;
                else
                    result -= pow_val;
            }
            last_d = d;
            cur_coef = (k & 1) ? 1 : -1;  /* start new coefficient */
        } else {
            /* continue current group */
            if (!has_last) {
                last_d = d;
                has_last = true;
                cur_coef = (k & 1) ? 1 : -1;
            } else {
                cur_coef += (k & 1) ? 1 : -1;
            }
        }
    }

    /* Flush the final group */
    if (has_last && cur_coef != 0) {
        seq_t pow_val = big_pow((seq_t)last_d, (seq_t)n);
        if (cur_coef > 0)
            result += pow_val;
        else
            result -= pow_val;
    }
    return result;
}
