/*
 * A344235  Triangle T from the array A(k, n) giving the sums of k+1
 *          consecutive squares starting with n^2, read as upwards
 *          antidiagonals, for k >= 0 and n >= 0.
 * https://oeis.org/A344235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Sum_{j=0..k} (l+j)^2 where s = floor((sqrt(8*n + 1) -
 *            1)/2), offset = n - s*(s + 1)/2, k = s - offset, l = offset.
 *            This implements the anti‑diagonal reading of the triangle
 *            A(k,n)=∑_{j=0..k}(n+j)^2
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Sum of squares of consecutive integers from a to b (a ≤ b). */
static inline seq_t
sum_squares(idx_t a, idx_t b)
{
    if (a == 0)
    {
        seq_t bb = (seq_t)b;
        return bb * (bb + 1) * (2 * bb + 1) / 6;
    }
    else
    {
        seq_t aa = (seq_t)a;
        seq_t bb = (seq_t)b;
        seq_t sum_b  = bb * (bb + 1) * (2 * bb + 1);
        seq_t aa_mm1 = aa - 1;
        seq_t sum_a_1 = aa_mm1 * aa * (2 * aa - 1);
        return (sum_b - sum_a_1) / 6;
    }
}

seq_t a(idx_t n)
{
    /* Compute s = floor((sqrt(8*n+1)-1)/2). */
    seq_t t = (seq_t)8 * (seq_t)n + 1;
    seq_t r = floor_sqrt(t);
    idx_t s = (idx_t)((r - 1) / 2);

    /* Number of terms up to the previous anti‑diagonal. */
    seq_t prev_len = (seq_t)s * ((seq_t)s + 1) / 2;
    seq_t offset  = (seq_t)n - prev_len;  /* 0 ≤ offset ≤ s */

    idx_t k = s - (idx_t)offset;   /* row index in the triangle */
    idx_t l = (idx_t)offset;       /* column index in the triangle */

    /* Sum of squares from l to l+k. */
    return sum_squares(l, l + k);
}
