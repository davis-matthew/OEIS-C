/*
 * A326286  a(n) equals the coefficient of x^(n*(n+1)) in Sum_{m>=0} (m+1) *
 *          x^m * (1 + x^m)^m / (1 + x^(m+1))^(m+2) for n >= 0.
 * https://oeis.org/A326286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(n*(n+1)) in Sum_{k>=0} (k+1) * x^k * (1
 *            + x^k)^k / (1 + x^(k+1))^(k+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local helper: binomial(n,k) for n up to ~1e8 and k up to sqrt(n)
   works with the arbitrary‑precision type seq_t.           */
static seq_t binom_small(seq_t n, idx_t k)
{
    if (n < (seq_t)k) return 0;
    if (k == 0 || n == (seq_t)k) return 1;

    /* use the smaller of k and n-k to minimise the loop size   */
    seq_t n_minus_k = n - (seq_t)k;
    if (n_minus_k < (seq_t)k)
        k = (idx_t)n_minus_k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - (seq_t)k + (seq_t)i);
        res = res / (seq_t)i;
    }
    return res;
}

/* main sequence routine                                                */
seq_t a(idx_t n)
{
    /* exponent to be extracted: N = n*(n+1)                            */
    uint64_t N = (uint64_t)n * (uint64_t)(n + 1);

    /* intermediates that are summed with sign                         */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* iterate over all possible m (0 .. N)                            */
    for (uint64_t m = 0; m <= N; ++m) {
        uint64_t m_plus = m + 1;
        uint64_t r = N % m_plus;                     /* N mod (m+1)   */
        uint64_t j_plus = (m_plus - r) % m_plus;
        if (j_plus == 0) j_plus = m_plus;            /* j+1            */
        uint64_t j = j_plus - 1;                     /* j              */

        int64_t numer = (int64_t)N - (int64_t)m * (int64_t)j_plus;
        if (numer < 0) continue;                     /* t would be negative */

        uint64_t t = numer / m_plus;                 /* signed integer  */
        int sign = (t & 1) ? -1 : 1;

        /* compute binomial factors                               */
        seq_t bin1 = binom_small((seq_t)m, (idx_t)j);                 /* binom(m,j)   */
        seq_t n_big = (seq_t)m + (seq_t)t + 1;
        seq_t bin2 = binom_small(n_big, (idx_t)(m_plus));            /* binom(m+t+1, m+1) */

        seq_t contrib = (seq_t)m_plus * bin1 * bin2;

        if (sign > 0)
            pos_sum += contrib;
        else
            neg_sum += contrib;
    }

    /* result should be non‑negative because a(n) is an integer      */
    return pos_sum - neg_sum;
}
