/*
 * A262437  Triangle T(n,k): write n in base 16, reverse order of digits.
 * https://oeis.org/A262437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 9785, u16 n <= 9785, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A262437
 * a(n) is the <n>-th element of the triangle T(n,k) that contains the
 * digits of n written in base 16, reversed.  In other words, what follows
 * is a row‑wise flattening of T:
 *
 *   T(0,0) , T(1,0) , ... , T(15,0) ,
 *   T(16,0), T(16,1), 
 *   T(17,0), T(17,1),
 *   T(18,0), T(18,1), ...
 *
 * The element returned is simply the hexadecimal digit in reversed
 * order belonging to the row r that contains the position n.
 *
 * The routine scans successive rows, computing the number of hex digits
 * in each row until the cumulative count passes n, then extracts the
 * appropriate digit.  This works for any non‑negative n and for any
 * integer type supplied for seq_t and idx_t by the build system.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the n-th element of A262437.  */
seq_t a(idx_t n)
{
    idx_t target = n;            /* index we are looking for */
    idx_t r = 0;                 /* current row number   */
    idx_t cum = 0;               /* cumulative digits so far */

    for (;; ++r) {
        /* Determine the number of hexadecimal digits in row r. */
        size_t len = 0;
        idx_t tmp = r;
        do {
            tmp /= (idx_t)16;
            ++len;
        } while (tmp > (idx_t)0);

        /* If the target lies within this row, extract the digit. */
        if (cum + len > target) {
            size_t k = (size_t)(target - cum);  /* column index within the row */

            idx_t t = r;
            for (size_t i = 0; i < k; ++i) {
                t /= (idx_t)16;
            }
            seq_t digit = (seq_t)(t % (idx_t)16);
            return digit;
        }

        /* Otherwise, skip this whole row and continue. */
        cum += (idx_t)len;
    }
}
