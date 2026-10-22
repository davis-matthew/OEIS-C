/*
 * A330920  Number of steps required to reach 1 for repeated applications of
 *          the Collatz-inspired function f(n) = 6*n+5-(n mod 5), or -1 if 1
 *          is never reached.
 * https://oeis.org/A330920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1: if n%5==0 then a(n)=a(n/5)+1 else
 *            a(n)=a((6*n+5-(n%5))/5)+2. This counts the number of
 *            applications of the Collatz‑inspired function f defined in the
 *            sequence
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    seq_t cur = (seq_t)n;
    seq_t steps = 0;
    /* Since cur may become larger than the original n, we perform
       the operation in steps of two (transform to a multiple of 5,
       then divide by 5) whenever cur is not divisible by 5. */
    while (cur != (seq_t)1) {
        seq_t r = cur % ((seq_t)5);
        if (r == 0) {
            cur = cur / ((seq_t)5);
            steps += 1;
        } else {
            /* (6*cur + 5 - r) is always divisible by 5 for r = cur%5 != 0 */
            cur = (((seq_t)6 * cur + ((seq_t)5) - r) / ((seq_t)5));
            steps += 2;
        }
    }
    return steps;
}
