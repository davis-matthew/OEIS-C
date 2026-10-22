/*
 * A341004  Numbers whose sum of even digits and sum of odd digits differ by
 *          3.
 * https://oeis.org/A341004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth positive integer x such that the absolute *
 *            difference between the sum of even digits and the sum of odd *
 *            digits of x is exactly 3. The sequence begins 3, 14, 25,
 * Range:     u8 n <= 26, u16 n <= 3967, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

/* Test whether a number satisfies the condition */
static bool satisfies(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);

    seq_t sumEven = 0, sumOdd = 0;
    for (int d = 0; d < 10; ++d) {
        seq_t contrib = (seq_t)d * freq[d];
        if (d % 2 == 0)
            sumEven += contrib;
        else
            sumOdd += contrib;
    }

    seq_t diff = (sumEven > sumOdd) ? (sumEven - sumOdd) : (sumOdd - sumEven);
    return diff == (seq_t)3;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t cur   = 0;

    if (n == 0) return 0;  /* undefined for n < 1 – defensive */

    while (true) {
        if (satisfies(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}
