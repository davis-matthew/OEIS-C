/*
 * A391207  The smallest nonnegative integer k such that n + k is not a sum of
 *          two squares.
 * https://oeis.org/A391207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A391207.
   a(n) = the smallest nonnegative integer k such that n + k is NOT a sum of two squares.
   The check is based on the theorem that an integer m is a sum of two squares iff
   in its prime factorisation all primes ≡3 (mod 4) occur with even exponent.
   Since among any four consecutive integers one is ≡3 (mod 4), k never exceeds 3. */

#include <stdbool.h>

static bool is_sum_of_two_squares(idx_t m)
{
    idx_t temp = m;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            do {
                temp /= p;
                exp++;
            } while (temp % p == 0);
            if ((p & 3) == 3 && (exp & 1))   // p ≡ 3 (mod 4) with odd exponent
                return false;
        }
    }
    if (temp > 1 && (temp & 3) == 3)          // remaining prime factor
        return false;
    return true;
}

seq_t a(idx_t n)
{
    for (idx_t k = 0; k <= 3; ++k) {
        idx_t m = n + k;
        if (!is_sum_of_two_squares(m))
            return (seq_t)k;
    }
    /* This point should never be reached because k <= 3 guarantees a solution. */
    return (seq_t)3;
}
