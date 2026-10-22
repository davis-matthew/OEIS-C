/*
 * A363804  a(n) = Sum_{d|n, d == 0, 3, or 4 mod 7} d.
 * https://oeis.org/A363804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d≡0,3,4 (mod 7)} d
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) without using floating point */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            idx_t mod = d1 % 7;
            if (mod == 0 || mod == 3 || mod == 4) {
                sum += (seq_t)d1;
            }

            if (d2 != d1) {
                mod = d2 % 7;
                if (mod == 0 || mod == 3 || mod == 4) {
                    sum += (seq_t)d2;
                }
            }
        }
    }
    return sum;
}
