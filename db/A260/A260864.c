/*
 * A260864  Base-14 representation of a(n) is the concatenation of the base-14
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose base‑14 representation is the
 *            concatenation of the base‑14 representations of 1,2,…,n,n−1,…,1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for the constexpr of 14 if needed */

static inline void append_number(idx_t k, seq_t *value)
{
    /* Concatenate the base‑14 digits of k into *value.
       Assumes k > 0. */
    idx_t t = k;
    int digits[20];          /* enough for any k < 2^64 in base 14 */
    int len = 0;

    /* Extract digits in reverse order (least significant first). */
    do {
        digits[len++] = t % 14;
        t /= 14;
    } while (t);

    /* Append digits from most significant to least. */
    for (int i = len - 1; i >= 0; --i) {
        *value = (*value) * (seq_t)14 + (seq_t)digits[i];
    }
}

seq_t a(idx_t n)
{
    seq_t val = 0;

    if (n == 0) {
        /* a(0) = 0 by definition */
        return 0;
    }

    /* Forward part: 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        append_number(k, &val);
    }

    /* Backward part: n‑1 .. 1 */
    if (n > 1) {
        for (idx_t k = n - 1; ; --k) {
            append_number(k, &val);
            if (k == 1) break;
        }
    }

    return val;
}
