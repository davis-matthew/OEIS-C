/*
 * A300081  a(n) is the number of steps needed to reach 1 or 89 under
 *          iteration of sum-of-squares-of-digits map.
 * https://oeis.org/A300081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of the sum‑of‑squares‑of‑digits map
 *            applied to n until the result becomes 1 or 89
 * Range:     u8 n <= 2647, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1 and 89 are already in the absorbing cycles */
    if (n == 1 || n == 89)
        return 0;

    seq_t cur = (seq_t)n;      /* current value of the iteration   */
    seq_t steps = 0;           /* iterations taken so far         */

    while (cur != 1 && cur != 89) {
        /* compute the sum of squares of decimal digits of cur */
        uint64_t sumsq = 0;
        seq_t tmp = cur;
        while (tmp != 0) {
            unsigned d = (unsigned)(tmp % 10);
            sumsq += (uint64_t)d * d;
            tmp /= 10;
        }
        cur = (seq_t)sumsq;    /* next value in the sequence   */
        ++steps;               /* one more step performed      */
    }
    return steps;
}
