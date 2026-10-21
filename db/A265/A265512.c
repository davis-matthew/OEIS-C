/*
 * A265512  a(n) = largest base-3 palindrome m <= n such that every base-3
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 3.
 * https://oeis.org/A265512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest base‑3 palindrome m ≤ n such that each base‑3
 *            digit of m is ≤ the corresponding digit of n. The value returned
 *            is the base‑3 representation written as a decimal integer (e.g.,
 *            base‑3 101 returns 101)
 * Range:     u8 n <= 27, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* seq_t and idx_t are defined elsewhere in the build. */
seq_t a(idx_t n)
{
    /* Special case for n == 0. */
    if (n == 0)
        return 0;

    /* Extract base‑3 digits of n, least significant first. */
    unsigned char d[64];      /* 3^41 > 2^64, enough for idx_t = uint64_t */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[len++] = tmp % 3;
        tmp /= 3;
    }

    /* The most significant digit of the palindrome must be ≤ both the
       most significant and the least significant digit of n.  If that
       bound is 0 the only valid palindrome is 0. */
    unsigned char msb_allowed = d[0] < d[len - 1] ? d[0] : d[len - 1];
    if (msb_allowed == 0)
        return 0;

    /* Build the palindrome digit by digit, producing its decimal
       representation. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned char digit = d[i] < d[len - 1 - i] ? d[i] : d[len - 1 - i];
        result = result * 10 + digit;
    }

    return result;
}
