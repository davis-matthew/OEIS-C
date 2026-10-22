/*
 * A374749  Number of zygodrome numbers in base-10 (A033023) with n or fewer
 *          digits.
 * https://oeis.org/A374749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=0, a(2)=9, a(3)=18; a(n)=2*a(n-1)+8*a(n-2)-9*a(n-3) for
 *            n>=4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* unused, but safe */
    if (n == 1) return 0;
    if (n == 2) return 9;
    if (n == 3) return 18;

    /* Use recurrence: a_n = 2*a_{n-1} + 8*a_{n-2} - 9*a_{n-3} */
    seq_t prev3 = 0;               /* a0 for recurrence (unused but set to 0) */
    seq_t prev2 = 0;               /* a1 */
    seq_t prev1 = 9;               /* a2 */

    seq_t current;
    for (idx_t i = 3; i <= n; ++i) {
        current = prev1 * 2 + prev2 * 8 - prev3 * 9;
        /* shift window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
