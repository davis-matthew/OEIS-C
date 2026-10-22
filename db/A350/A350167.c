/*
 * A350167  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^k.
 * https://oeis.org/A350167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/(2*k-1))^k
 * Range:     i8 n <= 26, i16 n <= 56, i32 n <= 117, i64 n <= 236,
 *            i128 n <= 482, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = ceil(n/2) must be the largest k with 2*k-1 ≤ n */
    idx_t m = (n + 1) / 2;
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k) {
        idx_t divisor = 2 * k - 1;
        idx_t f = n / divisor;          /* floor(n/(2*k-1)) */
        seq_t base = (seq_t)f;

        /* fast exponentiation of base^k */
        seq_t term = 1;
        idx_t e = k;
        while (e > 0) {
            if (e & 1) term *= base;
            base *= base;
            e >>= 1;
        }

        /* add or subtract depending on the sign */
        if (k & 1)      /* odd k: positive */
            sum += term;
        else            /* even k: negative */
            sum -= term;
    }

    return sum;
}
