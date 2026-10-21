/*
 * A270885  Irregular triangle read by rows, listing the digits 1,0,-1 in the
 *          representation of n > 0 in the binary balanced system (cf. comment
 *          in A268411).
 * https://oeis.org/A270885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, tabf
 * Formula:   not stated
 * Range:     i8 n <= 9537, i16 n <= 9537, i32 n <= 9537, i64 n <= 9537,
 *            i128 n <= 9537, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by generating the irregular triangle from the binary balanced
 * representation of each index.  Each row m is constructed by:
 *   for each set bit i in m: increment digit[i+1] by 1 and decrement digit[i] by 1.
 * The row consists of the digits from the highest non‑zero index down to 0
 * (including zeros).  The sequence is obtained by reading the rows
 * sequentially from left to right. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n<1, but return 0 */
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        /* digits[0..65] are sufficient for 64‑bit indices */
        seq_t digits[66] = {0};
        idx_t i = 0;
        idx_t temp = m;
        while (temp) {
            if (temp & 1) {
                digits[i + 1] += 1;
                digits[i] -= 1;
            }
            ++i;
            temp >>= 1;
        }
        /* i now equals the number of processed bits; highest index to output is i */
        for (idx_t j = i; ; --j) {
            seq_t d = digits[j];
            ++count;
            if (count == n) return d;
            if (j == 0) break;
        }
    }
}
