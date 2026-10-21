/*
 * A265557  a(n) = smallest base-9 palindrome m >= n such that every base-9
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 9.
 * https://oeis.org/A265557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decode decimal of the smallest base‑9 palindrome m ≥ n *
 *            with every digit of n ≤ the corresponding digit of m. * The
 *            function returns the palindrome read as a decimal integer
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n can be zero – treat it as a single zero digit. */
    size_t len = 0;
    if (n == 0) {
        len = 1;
    } else {
        /* Determine base‑9 length of n. */
        idx_t tmp = n;
        while (tmp != 0) {
            tmp /= 9;
            ++len;
        }
    }

    /* Allocate array for digits (LSB first). space < 32 is enough for 64‑bit n. */
    uint8_t d[32] = {0};
    uint8_t m[32] = {0};

    /* Extract base‑9 digits of n. */
    idx_t tmp = n;
    for (size_t i = 0; i < len; ++i) {
        d[i] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Build the minimal palindrome whose every digit is at least the
     * corresponding digit of n (treat missing higher digits of n as 0). */
    for (size_t i = 0; i < len; ++i) {
        size_t j = len - 1 - i;
        uint8_t dig = d[i];
        if (j < len && d[j] > dig)
            dig = d[j];
        /* Mirror the chosen digit. */
        m[i] = m[j] = dig;
    }

    /* Convert the palindrome digits (base‑9) to a decimal integer by
     * treating the digits as decimal digits. */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        /* Process from most significant to least. */
        ans = ans * (seq_t)10 + (seq_t)m[len - 1 - i];
    }

    return ans;
}
