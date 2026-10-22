/*
 * A381663  a(n) = A379342(A380200(n)).
 * https://oeis.org/A381663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2*(r-1)^2 - (r-1) + P(r,k) where r is the unique integer
 *            with 2r^2 - r ≥ n (the row) and k = n - (2*(r-1)^2 - (r-1))
 *            (column in that row). P(r,k) is defined by m = 2r-1 if (k < m) {
 *            if (k is odd) P = m - k; else P = 2*m - k; } else { if (k is
 *            odd) P = k; else P = k - m; } This directly imp...
 * Range:     u8 n <= 232, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, but safe */
    /* Find row r */
    idx_t r = 1;
    /* Use while loop: r increases until 2r^2 - r >= n */
    while ((2 * r * r - r) < n)
        r++;
    /* previous total terms up to row r-1 */
    idx_t prev = 2 * (r - 1) * (r - 1) - (r - 1);
    idx_t k = n - prev;            /* column (1‑based) */
    seq_t m = (seq_t)2 * (seq_t)r - 1;   /* m = 2r-1 */
    seq_t P;
    if (k < m) {
        if (k & 1)   /* odd */
            P = m - (seq_t)k;
        else          /* even */
            P = 2 * m - (seq_t)k;
    } else {
        if (k & 1)   /* odd */
            P = (seq_t)k;
        else          /* even */
            P = (seq_t)k - m;
    }
    seq_t base = (seq_t)2 * (seq_t)(r - 1) * (seq_t)(r - 1) - (seq_t)(r - 1);
    return base + P;
}
