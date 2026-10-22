/*
 * A385407  Number of strings of length n defined on {0, 1, 2, 3} that contain
 *          one or no 1's, two or no 2's, three or no 3's and any number of
 *          0's.
 * https://oeis.org/A385407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*binomial(n,6) + 10*binomial(n,5) + 4*binomial(n,4) +
 *            4*binomial(n,3) + binomial(n,2) + n + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    if (n >= 6)
        res += (seq_t)60 * binomial(n, 6);
    if (n >= 5)
        res += (seq_t)10 * binomial(n, 5);
    if (n >= 4)
        res += (seq_t)4  * binomial(n, 4);
    if (n >= 3)
        res += (seq_t)4  * binomial(n, 3);
    if (n >= 2)
        res += binomial(n, 2);

    res += (seq_t)n + 1;    /* n + 1  (seq_t cast ensures correctness) */

    return res;
}
