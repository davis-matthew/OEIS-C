/*
 * A329249  Starting from n: as long as the decimal representation starts with
 *          an odd number, multiply the largest such prefix by 2; a(n)
 *          corresponds to the final value.
 * https://oeis.org/A329249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) follows the iterative procedure described in the OEIS
 *            entry. Start with cur = n. While cur contains an odd decimal
 *            digit, find the longest suffix of even digits (v). Let u be the
 *            remaining leading part (an odd integer). Set cur = (u*2)*10^k +
 *            v, where 10^k is the length of v. When all digits...
 * Range:     u8 n <= 44, u16 n <= 2374, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case explicitly. */
    if (n == 0)
        return 0;

    /* Work with the sequence type from the build. */
    seq_t cur = (seq_t)n;

    for (;;) {
        /* Find the longest suffix of even digits. */
        seq_t u = cur;       /* leading part (prefix)          */
        seq_t suffix = 0;    /* accumulated suffix value       */
        seq_t power10 = 1;   /* 10^k for the current suffix    */

        /* Strip trailing even digits; stop when u becomes zero
           or the next (least significant) digit is odd. */
        while (u != 0 && (u % 10) % 2 == 0) {
            seq_t digit = u % 10;
            suffix = suffix + digit * power10;
            power10 = power10 * 10;
            u = u / 10;
        }

        /* If u == 0, all digits were even; stop. */
        if (u == 0)
            return cur;

        /* u is odd; perform the prescribed transformation. */
        cur = (u * 2) * power10 + suffix;
    }
}
