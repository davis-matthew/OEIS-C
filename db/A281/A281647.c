/*
 * A281647  Solutions x to the negative Pell equation x^2 - 10*y^2 = -6 with x
 *          > y > 0.
 * https://oeis.org/A281647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-2) - a(n-4) for n>4 a(1) = 2, a(2) = 22, a(3) =
 *            98, a(4) = 838 The sequence consists of the x‑values of
 *            solutions to x^2 - 10*y^2 = -6 with x > y > 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   mul38
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul38.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Base terms */
    if (n == 1) return 2;
    if (n == 2) return 22;
    if (n == 3) return 98;
    if (n == 4) return 838;

    /* For n > 4, use the second‑order recurrence separately for odd
       and even indices.  odd subsequence: a(1), a(3), a(5), …
       even subsequence: a(2), a(4), a(6), …                                       */
    if (n & 1ULL) {          /* odd n */
        idx_t k = (n + 1) / 2;   /* position in the odd subsequence */
        seq_t prev2 = 2;          /* a(1)  */
        seq_t prev1 = 98;         /* a(3)  */
        seq_t curr;
        for (idx_t i = 3; i <= k; ++i) {
            curr = mul38(prev1);
            curr -= prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    } else {                 /* even n */
        idx_t k = n / 2;          /* position in the even subsequence */
        seq_t prev2 = 22;         /* a(2)  */
        seq_t prev1 = 838;        /* a(4)  */
        seq_t curr;
        for (idx_t i = 3; i <= k; ++i) {
            curr = mul38(prev1);
            curr -= prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
}
