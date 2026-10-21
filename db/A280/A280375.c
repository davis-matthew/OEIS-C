/*
 * A280375  Expansion of Sum_{k>=1} k^3*x^(k^2)/(1 - x^k).
 * https://oeis.org/A280375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d <= sqrt(n)} d^3
 * Range:     u8 n <= 35, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This directly computes the sum of cubes of divisors of n that are
   not larger than the integer square root of n. */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but safe for n==0 */

    seq_t sum = 0;

    /* Find floor(sqrt(n)) using integer arithmetic */
    idx_t limit = 0;
    for (idx_t t = 1; t * t <= n; ++t)
        limit = t;

    /* Sum d^3 for all divisors d <= limit */
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            seq_t dd = (seq_t)d * d;     /* d squared as seq_t */
            seq_t cube = dd * (seq_t)d;  /* d cubed as seq_t */
            sum += cube;
        }
    }

    return sum;
}
