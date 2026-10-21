/*
 * A255368  a(n) = -(-1)^n * 2 * n / 3 if n divisible by 3, a(n) = -(-1)^n * n
 *          otherwise.
 * https://oeis.org/A255368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = -(-1)^n * n if n % 3 != 0; a(n) = -(-1)^n * 2*n/3 if n %
 *            3 == 0
 * Range:     i8 n <= 128, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* (-1)^n = 1 for even n, -1 for odd n.
       Therefore -(-1)^n = -1 for even n, +1 for odd n. */
    seq_t sign = ((n & 1ULL) == 0ULL) ? -1 : 1; /* even → -1, odd → +1 */
    if (n % 3ULL == 0ULL)
        return sign * ((nn * 2) / 3);
    else
        return sign * nn;
}
