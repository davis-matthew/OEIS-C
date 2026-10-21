/*
 * A249356  8*A200975(n)-7 where A200975 are the numbers on the diagonals in
 *          Ulam's spiral.
 * https://oeis.org/A249356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*A200975(n)-7 Recurrence: a(1)=1, a(2)=17 For n ≥ 3: if
 *            n mod 4 == 2 a(n) = 2*a(n-1) - a(n-2) + 16 else a(n) = 2*a(n-1)
 *            - a(n-2)
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1, but guard for 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)17;

    seq_t prev2 = (seq_t)1;               /* a(1) */
    seq_t prev1 = (seq_t)17;              /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        if ((i & 3ULL) == 2ULL)          /* i ≡ 2 (mod 4) */
            curr = (seq_t)2 * prev1 - prev2 + (seq_t)16;
        else
            curr = (seq_t)2 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
