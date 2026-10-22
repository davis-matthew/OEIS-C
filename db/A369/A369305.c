/*
 * A369305  Number of terms in A343524 that are less than 10^n.
 * https://oeis.org/A369305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=1..n+1} binomial(9, floor(k/2)) For n >= 18 the
 *            sum equals 1023
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Values bin[0..9] = binomial(9, k) for k=0..9 */
    static const seq_t bin[10] = {1, 9, 36, 84, 126, 126, 84, 36, 9, 1};

    if (n >= 18)              /* the sequence saturates at 1023 */
        return (seq_t)1023;

    seq_t sum = 0;
    idx_t limit = n + 1;      /* n < 18, so n+1 fits in idx_t without overflow */

    for (idx_t k = 1; k <= limit; ++k) {
        idx_t f = k / 2;      /* floor(k/2) */
        sum += bin[f];
    }
    return sum;
}
