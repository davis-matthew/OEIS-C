/*
 * A265525  a(n) = largest base-10 palindrome m <= n such that every base-10
 *          digit of m is <= the corresponding digit of n.
 * https://oeis.org/A265525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = largest base‑10 palindrome ≤ n whose digits are ≤ the
 *            corresponding digits of n. For numbers with a leading zero after
 *            construction the result is 0, because such a number cannot be a
 *            non‑zero palindrome
 * Range:     u8 n <= 261, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* digits of n in most–significant order, up to 20 digits for 64‑bit n */
    unsigned char d[20] = {0};
    unsigned char pal[20] = {0};
    size_t len = 0;
    idx_t tmp = n;

    /* Special case n == 0 */
    if (tmp == 0) {
        len = 1;
        d[0] = 0;
    } else {
        /* collect digits in reverse order */
        while (tmp > 0 && len < sizeof(d)) {
            d[len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        /* reverse into d so that d[0] is most significant */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char x = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = x;
        }
    }

    /* Build palindrome by mirroring with digitwise bound */
    for (size_t i = 0; i < len; ++i) {
        size_t j = len - 1 - i;
        if (i < j) {
            unsigned char v = (d[i] < d[j]) ? d[i] : d[j];
            pal[i] = pal[j] = v;
        } else if (i == j) {
            pal[i] = d[i];
        }
    }

    /* Leading zero implies the only non‑zero palindrome would have
       been invalid; return 0. */
    if (pal[0] == 0) {
        return (seq_t)0;
    }

    /* Convert digit array to numeric value */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * ((seq_t)10) + ((seq_t)pal[i]);
    }

    return result;
}
