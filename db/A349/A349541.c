/*
 * A349541  E.g.f.: exp(x) * (BesselI(0,8*x) + BesselI(1,8*x)).
 * https://oeis.org/A349541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*binomial(k, floor(k/2))*4^k
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result   = 0;
    seq_t pow4     = 1;    /* 4^k */
    seq_t binom_nk = 1;    /* binomial(n,0) */
    seq_t central  = 1;    /* binomial(k,floor(k/2)) for k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binom_nk * central * pow4 */
        seq_t term = binom_nk;
        term = term * central;
        term = term * pow4;
        result = result + term;

        if (k == n) break;

        /* update binomial(n, k+1) */
        binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);

        /* update central coefficient for next k */
        if ((k & 1) == 0) {          /* k even: (k+1)/(k/2+1) */
            seq_t num = (seq_t)(k + 1);
            seq_t den = (seq_t)(k / 2 + 1);
            central = central * num / den;
        } else {                     /* k odd: multiply by 2 */
            central = central * (seq_t)2;
        }

        /* update 4^k */
        pow4 = pow4 * (seq_t)4;
    }

    return result;
}
