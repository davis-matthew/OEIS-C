/*
 * A295220  a(n) = Sum_{i=1..floor(n/2)} floor((n+i)/i) - floor((n-i-1)/i).
 * https://oeis.org/A295220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} floor((n+i)/i) - floor((n-i-1)/i)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t first  = ((seq_t)(n + i)) / ((seq_t)i);
        seq_t second = ((seq_t)(n - i - 1)) / ((seq_t)i);
        sum += first - second;
    }
    return sum;
}
