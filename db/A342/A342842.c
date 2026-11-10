/*
 * A342842  All positive integer solutions m of equation A342369^k(6*p - 2) =
 *          m*3 + 2, sorted by p and k in ascending order, p has higher
 *          priority than k. p and k are positive integers. "^k" means
 *          recursion here.
 * https://oeis.org/A342842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for p = 1, 2, ...: x = 6p-2; repeat x = A342369(x) until x % 3
 *            == 0; each x with x % 3 == 2 along the way gives the term m =
 *            (x-2)/3. The terms are listed in order of p, then k. Computed by
 *            direct simulation
 * Range:     u8 n <= 38, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t A342369_step(seq_t x)
{
    if (x % 3 == 2)
        return (2 * x - 1) / 3;
    return 2 * x;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t p;
    for (p = 1; ; p++) {
        seq_t x = 6 * p - 2;
        for (;;) {
            x = A342369_step(x);
            if (x % 3 == 0)
                break;
            if (x % 3 == 2) {
                count++;
                if (count == n)
                    return (x - 2) / 3;
            }
        }
    }
}
