/*
 * A321726  Reverse each run of nonzero digits in ternary expansion of n and
 *          convert back to decimal.
 * https://oeis.org/A321726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8912, u32 n <= 8912, u64 n <= 8912,
 *            u128 n <= 8912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321726: a(n) is obtained by reversing each maximal run of non‑zero digits
   in the ternary expansion of n.  It satisfies the recurrence a(3*n) = 3*a(n). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 */

    char digits[64];
    int len = 0;
    idx_t tmp = n;

    /* convert n to base‑3, storing least–significant digit first */
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    /* reverse to most‑significant order */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        char t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }

    /* reverse each maximal run of non‑zero digits */
    for (int i = 0; i < len; ) {
        if (digits[i] == 0) {
            ++i;
            continue;
        }
        int start = i;
        while (i < len && digits[i] != 0)
            ++i;
        int end = i - 1;
        for (int l = start, r = end; l < r; ++l, --r) {
            char t = digits[l];
            digits[l] = digits[r];
            digits[r] = t;
        }
    }

    /* convert back to decimal */
    seq_t result = 0;
    for (int i = 0; i < len; ++i)
        result = result * 3 + (seq_t)digits[i];

    return result;
}
