/*
 * A348166  a(n) = abs(A020338(n)-A338754(n)).
 * https://oeis.org/A348166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |(n concatenated with n) – (n with each decimal digit
 *            duplicated)| For n < 10 the difference is 0. The function works
 *            for any integer type for idx_t (uint64_t) and any
 *            8/16/32/64/128‑bit seq_t
 * Range:     u8 n <= 9, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used by OEIS, just safety */

    /* Extract decimal digits of n, least significant first. */
    unsigned char digits[20];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* Compute 10^len. */
    seq_t pow10 = 1;
    for (size_t i = 0; i < len; ++i)
        pow10 *= (seq_t)10;

    /* A020338(n) = n ⋅ (10^len + 1). */
    seq_t concat = (seq_t)n * (pow10 + (seq_t)1);

    /* D(n) = duplicate each digit of n. */
    seq_t dup = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t d = (seq_t)digits[i];
        dup = dup * 10 + d;
        dup = dup * 10 + d;
    }

    /* Return the absolute difference. */
    return (concat > dup) ? (concat - dup) : (dup - concat);
}
