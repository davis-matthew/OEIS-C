/*
 * A328176  a(n) is the maximal value of the expression d AND (n/d) where d
 *          runs through the divisors of n and AND denotes the bitwise AND
 *          operator.
 * https://oeis.org/A328176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1158, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A328176: a(n) = max_{d|n} (d & (n/d)) */
seq_t a(idx_t n)
{
    seq_t max = 0;
    for (idx_t i = 1; (seq_t)i * i <= (seq_t)n; ++i) {
        if (n % i == 0) {
            seq_t d  = (seq_t)i;
            seq_t nd = (seq_t)(n / i);
            seq_t v = d & nd;
            if (v > max) max = v;

            /* handle the paired divisor n/i when distinct from i */
            if (i != n / i) {
                seq_t d2 = (seq_t)(n / i);
                seq_t nd2 = (seq_t)i;
                seq_t v2 = d2 & nd2;
                if (v2 > max) max = v2;
            }
        }
    }
    return max;
}
