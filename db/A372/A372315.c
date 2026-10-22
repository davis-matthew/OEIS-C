/*
 * A372315  Expansion of e.g.f. exp( x - LambertW(-2*x)/2 ).
 * https://oeis.org/A372315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (2*k+1)^(k-1) * binomial(n,k) (with
 *            (2*0+1)^(-1)=1, (2*1+1)^0=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;          /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term;
        if (k == 0 || k == 1)
            term = (seq_t)1;        /* (2*0+1)^(-1)=1, (2*1+1)^0=1 */
        else {
            seq_t base = (seq_t)(2 * k + 1);
            term = pow_seq(base, k - 1);
        }

        sum += term * binom;

        if (k < n) {
            seq_t numerator   = binom * (n - k);
            seq_t denominator = k + 1;
            binom = numerator / denominator; /* exact division */
        }
    }

    return sum;
}
