/*
 * A324302  a(n) = [x^(n*(n+1))] Sum_{m>=0} x^m * (x^(m+1) + i)^m / (1 +
 *          i*x^(m+1))^(m+1), for n >= 0.
 * https://oeis.org/A324302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n*(n+1))] Sum_{m>=0} x^m * (x^(m+1) + i)^m / (1 +
 *            i*x^(m+1))^(m+1), equivalent to a(n) = Sum_{d | (n*(n+1)+1)}
 *            (-1)^{(m+r)/2 + r} * Delannoy(m,r), where d runs over all
 *            divisors of n*(n+1)+1, m = d - 1, r = ( (n*(n+1)+1) / d ) - 1,
 *            Delannoy(m,r) = Σ_{k=0}^{min(m,r)} binom(m,k) * binom(...
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 15, i64 n <= 29, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 175 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t delannoy(seq_t m, seq_t r)
{
    /* computes the Delannoy number D(m,r) = Σ_{k=0}^{min(m,r)} C(m,k) C(r,k) 2^k */
    seq_t min_lr = m < r ? m : r;
    seq_t a = 1;          /* C(m,0) */
    seq_t b = 1;          /* C(r,0) */
    seq_t pow2 = 1;       /* 2^0 */
    seq_t sum = 0;
    for (idx_t k = 0; k <= (idx_t)min_lr; ++k)
    {
        seq_t add = a * b * pow2;
        sum += add;
        if (k == (idx_t)min_lr) break;
        /* update binomial coefficients and powers of two */
        a = a * (m - k) / (k + 1);
        b = b * (r - k) / (k + 1);
        pow2 = pow2 * 2;
    }
    return sum;
}

static void add_divisor_contrib(seq_t E, uint64_t d, seq_t *res)
{
    /* contributions from a single divisor d of E (= n*(n+1)+1) */
    seq_t m = (seq_t)d - 1;
    seq_t other = (seq_t)(E / d);           /* integer division, E is odd */
    seq_t r = other - 1;

    /* sign = (-1)^{(m+r)/2 + r} ; m+r is even because m,r are even */
    uint64_t m_plus_r = (uint64_t)((m + r) >> 1);    /* (m+r)/2 */
    uint64_t e_parity = ((m_plus_r & 1) ^ ( (uint64_t)r & 1 ));
    seq_t sign = e_parity ? -1 : 1;

    seq_t D = delannoy(m, r);
    *res += sign * D;
}

seq_t a(idx_t n)
{
    /* compute a(n) = coefficient of x^{n*(n+1)} in the given generating function */
    seq_t N = (seq_t)n * ((seq_t)n + 1);   /* N = n*(n+1) */
    seq_t E = N + 1;                       /* E = n*(n+1)+1, always odd */

    seq_t result = 0;
    uint64_t E_u64 = (uint64_t)E;          /* fits in 64‑bit for n <= 10000 */
    for (uint64_t d = 1; d * d <= E_u64; d += 2)   /* iterate only odd divisors */
    {
        if (E_u64 % d == 0)
        {
            add_divisor_contrib(E, d, &result);
            uint64_t other = E_u64 / d;
            if (other != d && (other & 1))
                add_divisor_contrib(E, other, &result);
        }
    }
    return result;
}
