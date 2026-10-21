/*
 * A284823  Array read by antidiagonals: T(n,k) = number of primitive
 *          (aperiodic) palindromes of length n using a maximum of k different
 *          symbols (n >= 1, k >= 1).
 * https://oeis.org/A284823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) read by antidiagonals where T(n,k) = Σ_{d | n}
 *            μ(n/d) * k^{ceil(d/2)}. The pair (n,k) is obtained from the
 *            index n by traversing the table along anti‑diagonals. Indexing
 *            starts at 1
 * Range:     u8 n <= 59, u16 n <= 146, u32 n <= 394, u64 n <= 1067,
 *            u128 n <= 1275, big any n
 * Helpers:   my_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard types:  idx_t (uint64_t), seq_t (user defined integer type).  */
#include <stdint.h>
#include "../utils/my_mu.h"

/*  Helper:  compute k^e for seq_t types.  */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/*  Compute T(i,j) = Σ_{d | i} μ(i/d) * j^{ceil(d/2)}.  */
static seq_t compute_T(idx_t i, idx_t j)
{
    seq_t pos = 0;   /* positive contributions (μ = +1) */
    seq_t neg = 0;   /* negative contributions (μ = -1) */

    for (idx_t d = 1; d * d <= i; ++d) {
        if (i % d == 0) {
            idx_t d1 = d;
            seq_t term1 = pow_seq((seq_t)j, (d1 + 1) / 2);
            int mu1 = my_mu(i / d1);
            if (mu1 == 1) pos += term1;
            else if (mu1 == -1) neg += term1;

            idx_t d2 = i / d1;
            if (d2 != d1) {
                seq_t term2 = pow_seq((seq_t)j, (d2 + 1) / 2);
                int mu2 = my_mu(i / d2);   /* equivalent to μ(d1) */
                if (mu2 == 1) pos += term2;
                else if (mu2 == -1) neg += term2;
            }
        }
    }
    return pos - neg;   /* pos ≥ neg for integer results */
}

/*  Main sequence function:  a(idx_t n)  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* Find diagonal s such that n ≤ s*(s+1)/2. */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;

    /* Number of entries before this diagonal. */
    idx_t prior = (s - 1) * s / 2;
    idx_t pos_in_diag = n - prior;          /* 1 … s */

    idx_t i = pos_in_diag;                 /* row index */
    idx_t j = s - pos_in_diag + 1;          /* column index */

    return compute_T(i, j);
}
