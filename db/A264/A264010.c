/*
 * A264010  Number of ways to write n as x^2 + y*(y+1) + z*(z+1)/2, where x, y
 *          and z are nonnegative integers such that y or y+1 is prime, and z
 *          or z+1 is prime.
 * https://oeis.org/A264010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x^2 + y*(y+1) + z*(z+1)/2 with
 *            x,y,z ≥ 0, (y or y+1) prime, and (z or z+1) prime
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t y = 1; ; ++y)
    {
        seq_t y_term = (seq_t)y * (y + 1);
        if (y_term > n) break;
        if (!(is_prime(y) || is_prime(y + 1))) continue;

        for (idx_t z = 1; ; ++z)
        {
            seq_t z_term = ((seq_t)z * (z + 1)) / 2;
            seq_t sum_y_z = y_term + z_term;
            if (sum_y_z > n) break;
            if (!(is_prime(z) || is_prime(z + 1))) continue;

            seq_t rem = (seq_t)n - sum_y_z;
            seq_t x = floor_sqrt(rem);
            if (x * x == rem) ++count;
        }
    }
    return count;
}
