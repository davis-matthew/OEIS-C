/*
 * A352227  Number of length-n blocks in the Thue-Morse sequence with
 *          intertwining pattern AB AB AB... .
 * https://oeis.org/A352227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n>=2, a(n) = 2*A006165(n-1), where
 *            A006165(1)=A006165(2)=1, A006165(2m)=2*A006165(m),
 *            A006165(2m+1)=A006165(m+1)+A006165(m)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* computes f = A006165(m), g = A006165(m+1), for m >= 1 */
static void A006165_pair(idx_t m, seq_t *f, seq_t *g) {
    if (m == 1) { *f = 1; *g = 1; return; }
    if (m == 2) { *f = 1; *g = 2; return; }
    seq_t x, y;
    A006165_pair(m / 2, &x, &y);
    if (m % 2 == 0) {
        *f = 2 * x;
        *g = x + y;
    } else {
        *f = x + y;
        *g = 2 * y;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t f, g;
    A006165_pair(n - 1, &f, &g);
    return 2 * f;
}
