/*
 * A370049  Square array A(n, k), n, k >= 0, read by antidiagonals; for any n
 *          and k >= 0 with respective binary expansions Sum_{i > 0}
 *          b_i*2^(i-1) and Sum_{i > 0} c_i*2^(i-1), the binary expansion of
 *          A(n, k) is Sum_{i > 0} d_i*2^(i-1) with d_i = (Sum_{k divides i}
 *          b_k*c_{i/k}) mod 2 for any i > 0.
 * https://oeis.org/A370049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are the anti‑diagonal coordinates of
 *            index n in the square array defined by the Dirichlet convolution
 *            of binary digits. For n,k ≥ 0, A(n,k) = XOR_{p:bit_p(n)=1}\;
 *            XOR_{q:bit_q(k)=1}\; (1 << (p*q-1)). This implements OEIS
 *            sequence A370049
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the coordinates (i,j) of index n in the anti‑diagonal ordering. */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t s = 0;
    /* Find largest s such that s*(s+1)/2 ≤ n. */
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    idx_t t = s * (s + 1) / 2;
    *i = n - t;        /* row index   */
    *j = s - *i;       /* column index */
}

/* Compute A(i,j) as defined above. */
static seq_t compute_A(idx_t i, idx_t j)
{
    seq_t result = 0;
    idx_t i_bits = i;
    idx_t bitpos_i = 1;
    while (i_bits) {
        if (i_bits & 1) {
            idx_t j_bits = j;
            idx_t bitpos_j = 1;
            while (j_bits) {
                if (j_bits & 1) {
                    idx_t pos = bitpos_i * bitpos_j;          /* position of the bit (1‑based) */
                    result ^= ((seq_t)1) << (pos - 1);        /* toggle that bit */
                }
                j_bits >>= 1;
                ++bitpos_j;
            }
        }
        i_bits >>= 1;
        ++bitpos_i;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return compute_A(i, j);
}
