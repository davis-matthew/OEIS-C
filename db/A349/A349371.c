/*
 * A349371  Inverse Möbius transform of Kimberling's paraphrase, A003602.
 * https://oeis.org/A349371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003602(d) A003602(d) is given by if d is odd
 *            then (d+1)/2 else A003602(d/2)
 * Range:     u8 n <= 314, u16 n <= 8818, u32 n <= 8818, u64 n <= 8818,
 *            u128 n <= 8818, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safeguard, though n >= 1 in usage */

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* compute f(i) */
            idx_t t = i;
            while ((t & 1) == 0) t >>= 1;
            seq_t f_i = ((seq_t)t + 1) / 2;
            sum += f_i;

            idx_t j = n / i;
            if (j != i) {
                /* compute f(j) */
                idx_t u = j;
                while ((u & 1) == 0) u >>= 1;
                seq_t f_j = ((seq_t)u + 1) / 2;
                sum += f_j;
            }
        }
    }
    return sum;
}
