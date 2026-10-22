/*
 * A368952  Irregular triangle T(n,k) read by rows: row n lists the larger
 *          number in each pair of triangular numbers (a, b) satisfying a - b
 *          = n.
 * https://oeis.org/A368952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368952 – Irregular triangle T(n,k) read by rows.
 *  Enumerates the larger triangular numbers T(x) appearing in pairs
 *  (T(x),T(y)) with difference d = T(x)−T(y) ≥ 1.  The triangular
 *  numbers of each difference d are listed in descending order,
 *  and the whole triangle is flattened row by row to form the
 *  sequence a(1),a(2),…  The function below generates the nth term
 *  of this sequence without pre‑computing the whole triangle.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer, return
 * floor(sqrt(n)).  No intrinsics or math.h are used, and
 * overflow is avoided by comparing mid with n / mid. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Sequence entry a(n) for A368952 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for indices < 1 */

    idx_t target = n;              /* flattened index to reach */
    idx_t count  = 0;              /* terms emitted so far */

    for (idx_t d = 1; ; ++d) {     /* iterate over difference rows */
        idx_t m = 2 * d;           /* m = 2*d, divisor product u*v */

        /* u runs through divisors of m, u < v, opposite parity */
        idx_t limit = isqrt_u64(m);

        for (idx_t u = 1; u <= limit; ++u) {
            if (m % u != 0) continue;          /* not a divisor */

            idx_t v = m / u;                    /* complementary divisor */
            if (v <= u) continue;               /* require u < v   */
            if (((u ^ v) & 1u) == 0) continue;  /* parity must differ → a odd */

            /* compute larger triangular number T(x) */
            idx_t a_val = u + v;                 /* a = u+v , odd   */
            idx_t x = (a_val - 1) >> 1;          /* x = (a-1)/2     */
            idx_t tri_i = x * (x + 1) / 2;       /* triangular number */
            seq_t tri = (seq_t)tri_i;

            ++count;
            if (count == target) return tri;
        }
    }
}
