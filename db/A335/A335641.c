/*
 * A335641  Number of ordered ways to write 2n+1 as p + x*(9x+7) with p prime
 *          and x an integer.
 * https://oeis.org/A335641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to express 2n+1 as p + x*(9x+7)
 *            with p prime and x integer
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9950 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* index starts at 1 */
    seq_t S = (seq_t)n * 2 + 1;           /* 2n+1 */
    seq_t limit = S > 2 ? S - 2 : 0;       /* p must be >=2 */
    seq_t count = 0;

    for (idx_t dx = 0; ; ++dx) {
        seq_t dd = (seq_t)dx;
        seq_t fneg = 9 * (dd * dd) - 7 * dd;   /* x = -dx */
        if (fneg > limit) break;              /* fneg increases for dx>=1 */

        seq_t pneg = S - fneg;
        if (is_prime_seq(pneg)) ++count;       /* negative x */

        if (dx > 0) {
            seq_t fpos = 9 * (dd * dd) + 7 * dd;   /* x = +dx */
            if (fpos <= limit) {
                seq_t ppos = S - fpos;
                if (is_prime_seq(ppos)) ++count;  /* positive x */
            }
        }
    }
    return count;
}
