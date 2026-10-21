/*
 * A289098  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 545", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) for n > 5
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with a(0) = 1, a(1) = 2, a(2) = 7, a(3) = 14, a(4) = 29, a(5) = 62 */
#include <stddef.h>

seq_t a(idx_t n) {
    const seq_t v0 = 1, v1 = 2, v2 = 7, v3 = 14, v4 = 29, v5 = 62;
    if (n == 0) return v0;
    if (n == 1) return v1;
    if (n == 2) return v2;
    if (n == 3) return v3;
    if (n == 4) return v4;
    if (n == 5) return v5;
    seq_t a_nm3 = v3;  /* a[n-3] */
    seq_t a_nm2 = v4;  /* a[n-2] */
    seq_t a_nm1 = v5;  /* a[n-1] */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * a_nm1 + a_nm2 - 2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
