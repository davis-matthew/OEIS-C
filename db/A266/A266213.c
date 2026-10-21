/*
 * A266213  Square array A(n,r), the number of neighbors at a sharp Manhattan
 *          distance r in a finite n-hypercube lattice, read by upwards
 *          antidiagonals; A(n,r) = Sum_{k=0..min(n,r)}
 *          binomial(r-1,k-1)*binomial(n,k)* 2^k.
 * https://oeis.org/A266213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   * a(n) = A(s - pos, pos), where * s = largest integer with
 *            s*(s+1)/2 <= n * pos = n - s*(s+1)/2 * A(n,r) = 1 if r == 0 *
 *            sum_{k=1..min(n,r)} binomial(r-1, k-1) * binomial(n, k) * 2^k
 * Range:     u8 n <= 47, u16 n <= 126, u32 n <= 418, u64 n <= 1508,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Translate 1‑dimensional index to (n,r) on the antidiagonals. */
    uint64_t idx = (uint64_t)n;

    /* Integer binary search for s such that s*(s+1)/2 <= idx < (s+1)*(s+2)/2.
       For idx up to about 10⁹ a simple loop is fast enough (s <= 44721). */
    uint64_t s = 0;
    uint64_t tri = 0;          /* triangular number s*(s+1)/2 */
    while (tri + s + 1 <= idx) {
        s++;
        tri += s;
    }

    uint64_t pos = idx - tri;          /* 0 <= pos <= s */
    uint64_t n_val = s - pos;          /* row index */
    uint64_t r_val = pos;              /* column index */

    /* Handle the trivial case r == 0. */
    if (r_val == 0) {
        return (seq_t)1;
    }

    /* Compute A(n,r) by the closed* form. */
    idx_t nn = (idx_t)n_val;
    idx_t rr = (idx_t)r_val;
    idx_t kmax = nn < rr ? nn : rr;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t term = binomial(rr - 1, k - 1) * binomial(nn, k) * pow2(k);
        sum += term;
    }
    return sum;
}
