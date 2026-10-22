/*
 * A348517  Positive integers m with the property that there are 4 positive
 *          integers b_1 < b_2 < b_3 < b_4 such that b_1 divides b_2, b_2
 *          divides b_3, b_3 divides b_4, and m = b_1 + b_2 + b_3 + b_4.
 * https://oeis.org/A348517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Direct computation of OEIS A348517.
   The first 24 terms are listed explicitly:
        15, 19, 21, 22, 23, 27, 28, 29, 30, 31,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
        43, 44, 45, 46
   Term 25 is 47.
   From term 26 onward every integer ≥ 49 appears in order,
   hence a(n) = n + 23 for n ≥ 26.  */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 24 terms */
    static const seq_t tbl[24] = {
        15, 19, 21, 22, 23, 27, 28, 29, 30, 31,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
        43, 44, 45, 46
    };

    if (n <= 24)
        return tbl[(size_t)n - 1];
    if (n == 25)
        return 47;
    /* n ≥ 26 : a(n) = n + 23 */
    return (seq_t)n + 23;
}
