/*
 * A256009  Triangle read by rows: Largest cardinality of a set of Hamming
 *          diameter <= k in {0,1}^n, k <= n.
 * https://oeis.org/A256009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) is the largest clique size in the graph on {0,1}^r with
 *            edges between vertices of Hamming distance ≤ k, where r and k
 *            are recovered from the flattened triangle index n. The formula
 *            is: - if k ≥ r → 2^r - if k == r-1 → 2^(r-1) - if k ≤ r-2 and k
 *            even → Σ_{i=0}^{k/2} C(r,i) - if k ≤ r-2 and k...
 * Range:     u8 n <= 43, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Recover (r,k) from the flattened index */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* Handle the easy cases first */
    if (k >= r)                /* k == r, the whole space */
        return pow2(r);
    if (k == r - 1)            /* distance n-1, half the space */
        return pow2(r - 1);

    /* Now k <= r-2 */
    if ((k & 1) == 0) {        /* k even */
        idx_t half = k / 2;
        seq_t sum = 0;
        for (idx_t i = 0; i <= half; ++i)
            sum += binomial(r, i);
        return sum;
    } else {                   /* k odd */
        idx_t half = (k - 1) / 2;
        seq_t sum = 0;
        for (idx_t i = 0; i <= half; ++i)
            sum += binomial(r, i);
        sum += binomial(r - 1, half);
        return sum;
    }
}
