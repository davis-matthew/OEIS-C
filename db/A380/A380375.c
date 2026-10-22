/*
 * A380375  Delete all 0's from n, then replace each remaining digit k of n by
 *          A000005(k).
 * https://oeis.org/A380375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = delete all 0's from n, then replace each remaining digit
 *            d by the number of divisors of d (A000005(d)), and concatenate
 *            the digits
 * Range:     u8 n <= 410, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t tau_digit[10] = {0, 1, 2, 2, 3, 2, 4, 2, 4, 3};

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t result = 0;
    /* Extract base‑10 digits into a small array. */
    unsigned char digits[20];   /* 20 digits suffice for all 64‑bit values */
    int len = 0;

    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Process digits from most significant to least significant. */
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        if (d == 0) {
            continue;          /* skip zeros */
        }
        seq_t v = tau_digit[d];
        result = result * 10 + v;
    }

    return result;
}
