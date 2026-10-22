/*
 * A380133  Expansion of e.g.f. sqrt(1 + 2*x*exp(x)).
 * https://oeis.org/A380133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0: a(n) = Σ_{k=1..n} (-1)^(k-1) * (2k-3)!! *
 *            k^(n-k) * C(n,k)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 14, i64 n <= 22, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (2k-3)!! is the odd double factorial; for k=1 it equals 1.  */
static inline seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = exp;
    while (e) {
        if (e & 1U) result = result * b;
        b = b * b;
        e >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    seq_t sum = (seq_t)0;
    seq_t d = (seq_t)1;          /* double factorial (2k-3)!!   */
    seq_t binom = (seq_t)1;      /* binomial nCk                */
    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1U) d = d * (seq_t)(2 * k - 3);
        /* update binom: binom = binom * (n-k+1) / k   */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        seq_t power = _pow_seq((seq_t)k, n - k);
        seq_t term = d * power * binom;
        if ((k & 1U) == 0) term = -term;    /* (-1)^(k-1) */
        sum += term;
    }
    return sum;
}
