/*
 * A346619  Triangle read by rows: T(n,k) = 1 iff 4 divides binomial(n,k) but
 *          8 does not (0 <= k <= n).
 * https://oeis.org/A346619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if 4 | C(r,k) but 8 ∤ C(r,k); 0 otherwise. Here (r,k)
 *            are the row and column corresponding to the flattened index n: r
 *            = floor((√(8n+1)-1)/2), k = n - r(r+1)/2. The condition is
 *            equivalent to v₂(C(r,k)) = 2, where v₂ denotes exponent of 2 in
 *            a number. Using Kummer's theorem: v₂(C(r,k)) =...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static inline unsigned popcnt(idx_t v) {
    unsigned cnt = 0;
    while (v) {
        v &= v - 1; /* drop lowest set bit */
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* Find row r */
    long double val = sqrtl((long double)8 * n + 1);
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);
    /* Adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* Column k */
    idx_t k = n - r * (r + 1) / 2;

    /* Exponent of 2 in C(r,k) */
    unsigned v2 = popcnt(k) + popcnt(r - k) - popcnt(r);

    return v2 == 2 ? (seq_t)1 : (seq_t)0;
}
