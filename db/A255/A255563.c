/*
 * A255563  a(n) = -3 * n/4 if n divisible by 4, a(n) = -(-1)^n * n otherwise.
 * https://oeis.org/A255563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = -3 * n / 4 if n % 4 == 0, otherwise a(n) = -(-1)^n * n
 * Range:     i8 n <= 127, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 4ULL == 0ULL) {
        /* n divisible by 4: a(n) = -3*n/4 */
        return -( (seq_t)3 * (seq_t)n / 4 );
    } else {
        /* a(n) = -(-1)^n * n
           For even n (not divisible by 4)  ->  -n
           For odd  n                            ->  +n */
        if ((n & 1ULL) == 0ULL) {
            return -(seq_t)n;
        } else {
            return (seq_t)n;
        }
    }
}
