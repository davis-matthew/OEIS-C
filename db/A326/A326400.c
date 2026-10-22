/*
 * A326400  Expansion of Sum_{k>=1} k * x^(2*k) / (1 - x^(3*k)).
 * https://oeis.org/A326400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d == 2 (mod 3)} d
 * Range:     u8 n <= 278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is undefined – return 0.  */
    if (n < 1u) return 0;

    seq_t sum = 0;

    /* Enumerate all divisor pairs (d, n/d) by iterating up to sqrt(n). */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* small divisor is i, large divisor is n/i. */
            idx_t d_small = i;
            idx_t d_large = n / i;

            seq_t d_small_s = (seq_t)d_small;
            seq_t d_large_s = (seq_t)d_large;

            if (d_small == d_large) {               /* perfect square */
                if (d_small_s % 3 == 2) sum += d_small_s;
            } else {                                 /* two distinct divisors */
                if (d_large_s % 3 == 2) sum += d_small_s;
                if (d_small_s % 3 == 2) sum += d_large_s;
            }
        }
    }
    return sum;
}
