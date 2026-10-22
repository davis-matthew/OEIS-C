/*
 * A374735  a(n) is the least k > 0 such that n and k*n can be added without
 *          carries in decimal.
 * https://oeis.org/A374735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least k>0 such that n and k*n can be added without
 *            carries in decimal
 * Range:     u8 n <= 18, u16 n <= 2947, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle trivial case explicitly */
    if (n == 0) return 1;

    /* Strip trailing zeros: a(10*n) = a(n) */
    idx_t m = n;
    while ((m % 10) == 0) {
        m /= 10;
    }

    /* Working copy of the trimmed number in the appropriate type */
    seq_t sn = (seq_t)m;

    /* Search for the minimal k */
    for (idx_t k = 1;; ++k) {
        seq_t product = sn * (seq_t)k;

        seq_t tmp_n = sn;
        seq_t tmp_p = product;
        bool ok = true;

        /* Check each decimal digit for a carry */
        while (tmp_n > 0 || tmp_p > 0) {
            seq_t d_n = tmp_n % 10;
            seq_t d_p = tmp_p % 10;
            if (d_n + d_p >= 10) {   /* carry would occur */
                ok = false;
                break;
            }
            tmp_n /= 10;
            tmp_p /= 10;
        }

        if (ok) {
            return (seq_t)k;
        }
    }
}
