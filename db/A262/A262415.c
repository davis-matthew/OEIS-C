/*
 * A262415  Number of (n+1) X (3+1) 0..1 arrays with each row divisible by 3
 *          and column not divisible by 3, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) * (2^(n+3) - 2 * (n%2?2:1)) / 2 (equivalently,
 *            a(n) = 3^(n-1) * (2^(n+3)-2)/2 for even n a(n) = 3^(n-1) *
 *            (2^(n+3)-4)/2 for odd n) Initial terms: a(1)=6, a(2)=45,
 *            a(3)=270 …
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: repeated multiplication, suitable for all seq_t sizes. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for offset below 1 */

    /* Compute powers: 3^(n-1) and 2^(n+3) */
    seq_t pow3 = pow_seq_t((seq_t)3, n - 1);
    seq_t pow2 = pow_seq_t((seq_t)2, n + 3);

    /* Difference term: 2^(n+3) - 4 if n odd, else 2^(n+3) - 2 */
    seq_t diff;
    if (n & 1U)
        diff = pow2 - (seq_t)4;
    else
        diff = pow2 - (seq_t)2;

    /* Divide by 2 */
    diff /= (seq_t)2;

    /* Final result */
    return pow3 * diff;
}
