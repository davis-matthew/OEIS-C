/*
 * A365058  E.g.f. satisfies A(x) = exp(x * A(x)^3 * (1 + x/2 * A(x)^3)).
 * https://oeis.org/A365058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (1/2)^(n-k) * (3*n+1)^(k-1) *
 *            binomial(k, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* a(0) = 1 */
        return 1;

    const idx_t max_r = n / 2;          /* floor(n/2)   */
    seq_t sum = 0;

    /* base = 3*n+1 */
    seq_t base = (seq_t)(3 * n + 1);

    /* compute base^(n-1) */
    seq_t base_pow = 1;
    for (idx_t i = 0; i < n - 1; ++i)
        base_pow *= base;

    /* state variables for the recurrence over r = 0 .. max_r */
    seq_t cur_factor = 1;   /* P(n,r) = n*(n-1)*...*(n-r+1)  */
    seq_t cur_binom = 1;    /* B(r) = binomial(n-r, r)       */

    for (idx_t r = 0; r <= max_r; ++r)
    {
        /* term = P(n,r) * B(r) * base_pow / 2^r */
        seq_t t = cur_factor * cur_binom * base_pow;
        t >>= r;               /* divide by 2^r (exact division) */
        sum += t;

        if (r == max_r)           /* finished the last term */
            break;

        /* ---------- update for next r = r+1 ---------- */
        /* P(n,r+1) = P(n,r) * (n - r) */
        cur_factor *= (seq_t)(n - r);

        /* B(r+1) using the derived recurrence:
           B(r) = B(r-1) * (n - 2r + 2)(n - 2r + 1) / (r *(n - r + 1))
           Here r -> r+1
        */
        idx_t next_r = r + 1;
        seq_t num_left  = (seq_t)(n - 2 * next_r + 2); /* n - 2(r+1) + 2  */
        seq_t num_right = (seq_t)(n - 2 * next_r + 1); /* n - 2(r+1) + 1  */
        seq_t denom_left  = (seq_t)next_r;            /* r+1            */
        seq_t denom_right = (seq_t)(n - next_r + 1);  /* n - (r+1) + 1  */
        seq_t numerator   = num_left * num_right;
        seq_t denominator = denom_left * denom_right;

        cur_binom = cur_binom * numerator / denominator;

        /* base_pow for next r: divide by base once */
        base_pow /= base;
    }

    return sum;
}
