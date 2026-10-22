/*
 * A368164  Number of nondeterministic Dyck bridges of length 2*n.
 * https://oeis.org/A368164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of t^n in (1-6*t)/(sqrt(1-8*t)*(1-9*t)).
 *            The implementation computes the convolution a(n) = Σ_{k=0..n}
 *            B(n-k) * C(2k,k) * 2^k where B(0)=1, B(m)=3·9^{m-1} (m>0). All
 *            arithmetic is performed in seq_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t pow2 = 1;   /* 2^k */
    seq_t binom = 1;  /* C(2k,k) for current k */

    /* m = n - k, initial m = n */
    idx_t m = n;
    seq_t B;          /* current B(m) */
    if (m == 0)
        B = 1;
    else
        B = 3 * pow_seq((seq_t)9, (int)(m - 1));

    for (idx_t k = 0; k <= n; ++k) {
        /* term = B(m) * C(2k,k) * 2^k */
        seq_t D = binom * pow2;
        sum += B * D;

        if (k == n)
            break;

        /* Update binom to C(2(k+1),k+1) */
        seq_t num = binom * ((seq_t)(2 * k + 1));
        num = num * ((seq_t)(2 * k + 2));
        seq_t denom = ((seq_t)(k + 1)) * ((seq_t)(k + 1));
        binom = num / denom;

        pow2 = pow2 * 2;

        /* Advance B for next m = n - k - 1 */
        --m;
        if (m == 0)
            B = 1;
        else
            B = B / 9;
    }

    return sum;
}
