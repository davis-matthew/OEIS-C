/*
 * A333782  G.f.: Sum_{k>=1} (-1)^(k + 1) * k * x^(k^2) / (1 - x^k).
 * https://oeis.org/A333782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d | n, d <= sqrt(n)} (d if d is odd else -d)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = (sum of odd divisors of n not exceeding sqrt(n))
   minus (sum of even divisors of n not exceeding sqrt(n)). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0, but safe guard */

    seq_t sum_odd = 0;
    seq_t sum_even = 0;

    /* Iterate over all divisors d <= sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d & 1) {          /* d is odd */
                sum_odd += d;
            } else {              /* d is even */
                sum_even += d;
            }
        }
    }

    return sum_odd - sum_even;
}
