/*
 * A364232  Expansion of Sum_{k>=0} x^(3*k+1) / (1 + x^(3*k+1))^2.
 * https://oeis.org/A364232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, n/d == 1 mod 3} (-1)^(d+1)*d
 * Range:     i8 n <= 103, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n==0 (outside the OEIS range) simply return 0. */
    if (n == 0) return 0;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d, e;

            /* First divisor i */
            d = i;
            e = n / d;
            if ((e % 3) == 1) {
                if ((d & 1) == 0) sum -= (seq_t)d;
                else                sum += (seq_t)d;
            }

            /* Second divisor n/i, if different */
            if (i != e) {
                d = e;
                e = n / d;          /* e is now the complementary divisor i */
                if ((e % 3) == 1) {
                    if ((d & 1) == 0) sum -= (seq_t)d;
                    else                sum += (seq_t)d;
                }
            }
        }
    }
    return sum;
}
