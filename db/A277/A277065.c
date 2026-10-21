/*
 * A277065  Sum of cubes of the digits of all divisors of n.
 * https://oeis.org/A277065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = sum_{d|n} (sum of cubes of the decimal digits of d)
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed cubes of digits 0–9 (stored in unsigned to save space). */
    static const unsigned digit_cube[10] = {
        0, 1, 8, 27, 64, 125, 216, 343, 512, 729
    };

    seq_t sum = 0;                 /* accumulator for the result   */

    /* Enumerate all divisors of n by checking i up to sqrt(n). */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* Process divisor i.  Compute the sum of cubes of its digits. */
            seq_t temp = 0;
            idx_t x = i;
            while (x > 0)
            {
                unsigned d = x % 10;               /* current decimal digit */
                temp += (seq_t)digit_cube[d];
                x   /= 10;
            }
            sum += temp;

            /* If i is not the square root, process the paired divisor. */
            idx_t j = n / i;
            if (j != i)
            {
                temp = 0;
                x = j;
                while (x > 0)
                {
                    unsigned d = x % 10;
                    temp += (seq_t)digit_cube[d];
                    x   /= 10;
                }
                sum += temp;
            }
        }
    }

    return sum;
}
