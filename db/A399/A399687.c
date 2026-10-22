/*
 * A399687  Triangle read by rows: T(n,k) is the maximum number of colors that
 *          can be used in an edge-coloring of K_n containing no rainbow copy
 *          of the k-vertex cycle C_k, for 3 <= k <= n.
 * https://oeis.org/A399687
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows: T(n,k) = binom(k-1,2) *
 *            floor(n/(k-1)) + binom(n mod (k-1),2) + ceil(n/(k-1)) - 1 with 3
 *            ≤ k ≤ n. The sequence a starts at a(3)=T(3,3)
 * Range:     u8 n <= 277, u16 n <= 9905, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t binom2(seq_t x)
{
    return x * (x - 1) / 2;
}

static uint64_t isqrt_u64(uint64_t x)
{
    if (x <= 1) return x;
    long double d = sqrtl((long double)x);
    uint64_t r = (uint64_t)d;
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

seq_t a(idx_t idx)
{
    if (idx < 3) return 0;

    /* Position inside the flattened triangle (1‑based). */
    uint64_t m = idx - 2;

    /* Find the row n such that (n‑2)(n‑1)/2 ≥ m. */
    uint64_t delta = 1 + 8 * m;
    uint64_t root   = isqrt_u64(delta);
    uint64_t n = (3 + root) / 2;              /* initial guess */
    uint64_t tri = (n - 2) * (n - 1) / 2;
    while (tri < m) {
        ++n;
        tri = (n - 2) * (n - 1) / 2;
    }

    /* Offset within the current row. */
    uint64_t prev_tri = (n - 3) * (n - 2) / 2;
    uint64_t offset_in_row = m - prev_tri;          /* 1 … (n‑2) */
    seq_t k = (seq_t)(offset_in_row + 2);           /* k = 3 … n */

    seq_t k1 = k - 1;                               /* k-1 */
    seq_t term1 = binom2(k1) * ((seq_t)n / k1);     /* binom(k-1,2)*floor(n/(k-1)) */
    idx_t   rem_tmp = (idx_t)n % (idx_t)k1;          /* n mod (k-1) as idx_t */
    seq_t   rem = (seq_t)rem_tmp;                   /* cast to seq_t */
    seq_t term2 = binom2(rem);                      /* binom(n mod (k-1),2) */
    seq_t ceilv = ((seq_t)n + k1 - 1) / k1;          /* ceil(n/(k-1)) */

    return term1 + term2 + ceilv - 1;
}
