/*
 * A375797  Table T(n, k) read by upward antidiagonals. The sequences in each
 *          column k is a triangle read by rows (blocks), where each row is a
 *          permutation of the numbers of its constituents. Row number n in
 *          column k has length n*k = A003991(n,k); see Comments.
 * https://oeis.org/A375797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the table T(n,k) (OEIS A375797). The
 *            entry T(n,k) is defined by L = ceil((sqrt(8*n*k + k*k) - k) /
 *            (2*k)) R = n - k * (L-1) * L / 2 val = k*L + 1 - R P = (
 *            (max(R,val) is even) ? R : val ) T = P + k*(L-1)*L/2 a(n) is T
 *            evaluated at the pair (n,k) that is obtained by en...
 * Range:     u8 n <= 50, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root (floor) for an arbitrary unsigned integer type seq_t.
   Binary search with no overflow:  mid <= x / mid  ⇔  mid^2 <= x. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x + (seq_t)1;  /* exclusive upper bound */

    while (low + (seq_t)1 < high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0) { low = 0; high = 1; continue; }
        if (mid <= x / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Compute the table entry T(n,k) using only integer arithmetic.
   Parameters n and k are natural numbers ≥ 1. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t nr   = (seq_t)n;
    seq_t nc   = (seq_t)k;
    seq_t D    = (seq_t)8 * nr * nc + nc * nc;          /* 8*n*k + k^2 */

    seq_t sqrt_D = isqrt_seq(D);
    bool is_square = (sqrt_D * sqrt_D == D);

    seq_t numer = sqrt_D - nc;                         /* >= 0 */
    seq_t denom = (seq_t)2 * nc;                       /* 2*k */

    /* ceil division of (sqrt_D - k) by 2*k */
    seq_t L = numer / denom;
    if ( (numer % denom) != 0 || !is_square )
        L += 1;

    seq_t tri = nc * (L - (seq_t)1) * L / (seq_t)2;    /* k*(L-1)*L/2 */

    seq_t R    = nr - tri;
    seq_t val  = nc * L + (seq_t)1 - R;
    seq_t m    = (R > val) ? R : val;
    seq_t P    = ((m & (seq_t)1) == 0) ? R : val;

    return P + tri;
}

/* Map the overall index n (≥1) to the corresponding table coordinates
   (row, col) by the usual upward‑antidiagonal enumeration. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard */

    /* Find the smallest s such that s*(s-1)/2 ≥ n.
       s is the antidiagonal number (row+col). */
    idx_t s   = 1;
    idx_t cnt = 0;          /* number of elements before the current diagonal */

    while (cnt + (s - (idx_t)1) < n) {
        cnt += s - (idx_t)1;
        ++s;
    }

    idx_t offset = n - cnt - (idx_t)1;          /* 0‑based index within the diagonal */
    idx_t row    = s - (idx_t)1 - offset;       /* n‑coordinate */
    idx_t col    = s - row;                     /* k‑coordinate */

    return compute_T(row, col);
}
