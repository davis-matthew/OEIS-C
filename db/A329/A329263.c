/*
 * A329263  Irregular triangle read by rows in which row n is the result of
 *          iterating the operation f(n) = n/8 if n == 0 (mod 8), otherwise
 *          f(n) = 8*(floor(n/8) + n + 1), terminating at the first occurrence
 *          of 1.
 * https://oeis.org/A329263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n‑th term of the irregular triangle read by rows.
 *            Each row starts with the integer m and consists of successive
 *            iterates of f(x): f(x) = x/8 if x ≡ 0 (mod 8) 8*(⌊x/8⌋ + x + 1)
 *            otherwise The row stops when the first 1 is produced. a(n) is
 *            the n‑th number in this flattened list of rows
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t count = 0;                  /* number of terms produced so far   */
    for (idx_t start = 1; ; ++start) /* generate rows 1,2,3,…            */
    {
        seq_t val = (seq_t)start;     /* current value in the row         */
        while (true)
        {
            ++count;                  /* emit this term                   */
            if (count == n)           /* the desired term?                */
                return val;

            if (val == 1)              /* row finished – go to next start */
                break;

            /* next value of the row: f(val)                     */
            if ((val % (seq_t)8) == 0) /* divisible by 8?                  */
                val = val / (seq_t)8;
            else
                val = (seq_t)8
                    * ((val / (seq_t)8) + val + (seq_t)1);
        }
    }
}
