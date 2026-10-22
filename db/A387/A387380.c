/*
 * A387380  a(n) = number of strict quartets (n,u,v,w) of type 4; see
 *          Comments.
 * https://oeis.org/A387380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict quartets (n,u,v,w) of type 4
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t sn = (seq_t)n;
    /* iterate over all possible u: 1 <= u < n */
    for (idx_t ui = 1; ui < n; ++ui) {
        seq_t u = (seq_t)ui;
        seq_t su = sn - u;            /* n - u  > 0  because ui < n */
        seq_t lhs = sn * su;          /* n * (n - u)  */

        /* enumerate divisors d of lhs with 2 <= d <= sqrt(lhs) */
        for (seq_t d = 2; d * d <= lhs; ++d) {
            if (lhs % d != 0) continue;    /* d not a divisor */

            seq_t a_val = d;                /* v = a_val */
            seq_t b_val = lhs / d;          /* v + w = b_val */

            if (a_val >= b_val) continue;  /* need v < v + w */
            seq_t v = a_val;
            seq_t w = b_val - a_val;

            /* distinctness check: all four numbers must be different */
            if (v == sn || v == u || w == sn || w == u || w == v) continue;

            ++total;                       /* found a valid strict quartet */
        }
    }
    return total;
}
