/*
 * A363891  Sum of divisors of 3*n-2 of form 3*k+2.
 * https://oeis.org/A363891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of 3*n-2 with d ≡ 2 (mod 3)
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = 3*n - 2;            /* number to factor */
    seq_t sum = 0;                /* accumulator */

    for (idx_t d = 1; d*d <= m; ++d) {
        if (m % d == 0) {
            if (d % 3 == 2)
                sum += (seq_t)d;
            idx_t other = m / d;
            if (other != d && other % 3 == 2)
                sum += (seq_t)other;
        }
    }
    return sum;
}
