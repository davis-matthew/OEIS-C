/*
 * A345681  a(0) = 0; for n >= 1, a(n) = A004185(a(n-1)+n).
 * https://oeis.org/A345681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004185(a(n-1) + n), a(0) = 0 * where A004185(x) returns
 *            the integer obtained by sorting the decimal * digits of x in
 *            nondecreasing order (leading zeros are dropped)
 * Range:     u8 n <= 60, u16 n <= 2999, u32 n <= 2999, u64 n <= 2999,
 *            u128 n <= 2999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use standard integer types provided by the build. */
#include <stdlib.h>

/* Helper: reorder the digits of x in ascending order.  */
static seq_t ascending_digits(seq_t x)
{
    if (x == 0)
        return 0;                     /* 0 stays 0 */

    /* At most 40 decimal digits are needed for 128‑bit values. */
    int digits[40];
    int dcnt = 0;

    /* Extract digits in reverse order. */
    while (x > 0) {
        digits[dcnt++] = (int)(x % 10);
        x = x / 10;
    }

    /* Simple insertion sort (digits are small, performance is negligible). */
    for (int i = 0; i < dcnt - 1; ++i) {
        for (int j = i + 1; j < dcnt; ++j) {
            if (digits[i] > digits[j]) {
                int t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    /* Reconstruct the number from the sorted digits. */
    seq_t result = 0;
    for (int i = 0; i < dcnt; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t val = 0;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = ascending_digits(val + (seq_t)i);
    }
    return val;
}
