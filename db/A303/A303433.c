/*
 * A303433  "Wondrous representation" [right to left] of positive integer n, n
 *          >= 2.
 * https://oeis.org/A303433
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by running the Collatz iteration * from n to 1.
 *            For each step, output the digit 1 if the current * number is odd
 *            (3k+1 step) and 2 if it is even (k/2 step). * The digits are
 *            concatenated in the order of the steps, yielding * the "wondrous
 *            representation" of n. The function return...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t cur = n;      /* current value in the Collatz iteration   */
    seq_t res = 0;      /* the resulting digit string as an integer */

    /* Iterate until we reach 1.  For n>=2, the loop will terminate. */
    while (cur > 1) {
        seq_t digit = (cur & 1) ? 1 : 2;   /* 1 for odd, 2 for even */
        res = res * 10 + digit;            /* append the digit */

        /* Perform the Collatz step on cur */
        if (cur & 1) {                     /* odd -> 3k+1 */
            cur = cur * (seq_t)3 + 1;
        } else {                           /* even -> k/2 */
            cur >>= 1;                     /* divide by 2 */
        }
    }
    return res;
}
