/*
 * A364013  Expansion of Sum_{k>0} k^2 * x^k / (1 + x^(3*k)).
 * https://oeis.org/A364013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, n/d≡1 (mod 3)} (-1)^{n/d+1} · d² Equivalent to
 *            a(n) = -∑_{d|n, n/d≡1 (mod 3)} (-1)^{n/d} · d² Computes by
 *            iterating over all divisor pairs of n
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d = i;
            idx_t q = n / d;              /* quotient */
            if (q % 3 == 1) {
                if (q & 1)               /* q odd => + d² */
                    sum += (seq_t)d * (seq_t)d;
                else                     /* q even => - d² */
                    sum -= (seq_t)d * (seq_t)d;
            }

            /* counterpart divisor d2 = n / i (if distinct) */
            idx_t d2 = n / i;
            if (d2 != d) {
                idx_t q2 = n / d2;          /* = i */
                if (q2 % 3 == 1) {
                    if (q2 & 1)
                        sum += (seq_t)d2 * (seq_t)d2;
                    else
                        sum -= (seq_t)d2 * (seq_t)d2;
                }
            }
        }
    }
    return sum;
}
