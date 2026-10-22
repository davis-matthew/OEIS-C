/*
 * A329201  The ghost iteration (B): add or subtract the number formed by
 *          absolute differences of digits (A040115), according to parity (odd
 *          or even).
 * https://oeis.org/A329201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n - (-1)^d * d, where d is the number formed by the
 *            absolute differences of consecutive decimal digits of n
 *            (A040115). For single‑digit n (n < 10) we define d = 0
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;          /* convert index to seq_t for calculations */
    seq_t d = 0;                 /* the digit‑difference number */

    if (x >= 10) {               /* compute A040115(n) only for n >= 10   */
        seq_t prev = x % 10;     /* last digit (rightmost)                */
        seq_t rest = x / 10;     /* remaining part of the number          */

        while (rest > 0) {
            seq_t curr = rest % 10;                          /* next digit   */
            seq_t diff = (prev > curr) ? (prev - curr) : (curr - prev);
            d = d * 10 + diff;          /* build the difference number      */
            prev = curr;
            rest /= 10;
        }
    }

    /* Add or subtract d depending on its parity:
       if d is even:   a = n - d
       if d is odd:    a = n + d */
    return ((d & 1) == 0) ? (x - d) : (x + d);
}
