/*
 * A265556  a(n) = smallest base-9 palindrome m >= n such that every base-9
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑9 palindrome m ≥ n such that every base‑9
 *            digit of n * is ≤ the corresponding digit of m. The output m is
 *            written in base 10
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑9 digits (least significant first).  */
    unsigned char d[64];
    int len = 0;
    idx_t temp = n;
    if (temp == 0) {
        d[0] = 0;
        len  = 1;
    } else {
        while (temp > 0) {
            d[len++] = temp % 9u;
            temp /= 9u;
        }
    }

    /* Build the minimal palindrome satisfying the digit‑wise constraint. */
    unsigned char pal[64];
    for (int i = 0; i <= len / 2; ++i) {
        int j    = len - 1 - i;
        unsigned char maxv = d[i] > d[j] ? d[i] : d[j];
        pal[i] = pal[j] = maxv;
    }

    /* Convert the palindrome back to base‑10 value (in seq_t). */
    seq_t m = 0;
    for (int i = len - 1; i >= 0; --i) {
        m = m * 9 + pal[i];
    }
    return m;
}
