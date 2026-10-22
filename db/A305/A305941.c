/*
 * A305941  Smallest power of n having at least one digit 0 (when written in
 *          base 10).
 * https://oeis.org/A305941
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 57, u64 n <= 581, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_counts, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the smallest power of n (n >= 2) whose decimal representation
 * contains at least one digit 0.  The function repeatedly multiplies by
 * n until a zero digit is found.
 */
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    const seq_t base = (seq_t)n;
    seq_t power = base;
    unsigned char cnt[10];

    digit_counts(power, cnt);
    while (cnt[0] == 0)
    {
        power = seq_mul(power, base);
        digit_counts(power, cnt);
    }

    return power;
}
