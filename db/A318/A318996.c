/*
 * A318996  a(n) = Sum_{d|n} (sigma(n) mod d).
 * https://oeis.org/A318996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (sigma(n) mod d)
 * Range:     u8 n <= 63, u16 n <= 9414, u32 n <= 9414, u64 n <= 9414,
 *            u128 n <= 9414, big any n
 * Helpers:   floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t sum = 0;

    idx_t root = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d == 0)
        {
            sum += sigma % (seq_t)d;
            idx_t other = n / d;
            if (other != d)
                sum += sigma % (seq_t)other;
        }
    }
    return sum;
}
