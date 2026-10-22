/*
 * A393711  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers satisfying x <= y < z.
 * https://oeis.org/A393711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393711
 *  a(n) = number of triples (x, y, z) of positive integers such that
 *  x^2 + y*z = n, with x <= y < z
 *  The function iterates over all possible x (1 <= x <= sqrt(n)),
 *  then over all factor pairs (y, z) of n - x^2 with y <= sqrt(n - x^2),
 *  counting those that satisfy x <= y < z.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* iterate over x such that x^2 <= n */
    for (idx_t x = 1; x <= n / x; ++x) {
        idx_t t = n - x * x;          /* remaining product y*z */
        if (t == 0) {
            /* y*z = 0 has no positive solutions, skip */
            continue;
        }

        /* iterate over divisors y of t up to sqrt(t) */
        for (idx_t y = 1; y <= t / y; ++y) {
            if (t % y != 0) continue; /* not a divisor */

            idx_t z = t / y;            /* complementary divisor */
            if (y >= x && y < z) {
                ++count;                /* valid triple found */
            }
        }
    }

    return count;
}
