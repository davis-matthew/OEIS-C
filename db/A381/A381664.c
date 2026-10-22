/*
 * A381664  a(n) = A380200(A379342(n)).
 * https://oeis.org/A381664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (n-1)*(2n-3) + P(n,k) where n is the row number and k is
 *            the position of a(n) in that row (1 ≤ k ≤ 4n-3). The permutation
 *            P is defined by m = 2n-1 if k < m: if k is odd => P = m - k else
 *            => P = k + m - 1 else: if k is odd => P = k else => P = 2*m - k
 *            - 1 The row length is 4n-3 and the rows...
 * Range:     u8 n <= 234, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t index) {
    if (index == 0) return 0;   /* offset 1, unused but safe */

    /* Find row number r such that S(r-1) < index ≤ S(r),
       where S(r) = 2r² - r is the cumulative length of rows 1…r.   */
    double sqrtD = sqrt((double)1 + 8.0 * (double)index);
    idx_t r = (idx_t)((1.0 + sqrtD) / 4.0);  /* initial approximation */
    if (r == 0) r = 1;
    while ((2ULL * r * r - r) < index) r++;   /* refine if under‑estimation */

    /* Compute position k within the row r */
    idx_t prevS = 2ULL * (r - 1) * (r - 1) - (r - 1);  /* S(r-1) */
    idx_t k = index - prevS;                          /* 1‑based index */

    /* Permutation value P */
    idx_t m = 2 * r - 1;
    idx_t P;
    if (k < m) {
        if (k & 1)          /* k odd */
            P = m - k;
        else                /* k even */
            P = k + m - 1;
    } else {
        if (k & 1)          /* k odd */
            P = k;
        else                /* k even */
            P = 2 * m - k - 1;
    }

    /* Base offset contributed by the block start: (r-1)(2r-3)   */
    seq_t base;
    if (r <= 1)
        base = 0;
    else
        base = ((seq_t)(r - 1)) * ( (seq_t)(2 * r - 3) );

    return base + (seq_t)P;
}
