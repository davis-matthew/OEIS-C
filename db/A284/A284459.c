/*
 * A284459  Permutation of the positive integers: this permutation transforms
 *          the enumeration system of positive irreducible fractions
 *          A002487/A002487' (Calkin-Wilf) into the enumeration system
 *          A245327/A245328, and A162911/A162912 (Drib) into A020651/A020650
 *          (Yu-Ting inverted).
 * https://oeis.org/A284459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258996(A231551(n)); equivalently a(n) = the Calkin-Wilf
 *            index k with A002487(k)/A002487(k+1) = A245327(n)/A245328(n),
 *            where f(1) = 1, f(2n) = 1/(f(n)+1), f(2n+1) = f(n)+1
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 79 terms through n = 9512 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Step 1: compute f(n) = p/q (A245327/A245328) from the binary digits of n */
    seq_t p = 1, q = 1;
    int top = 63;
    while (top > 0 && !((n >> top) & 1)) top--;
    for (int i = top - 1; i >= 0; i--) {
        if ((n >> i) & 1) {          /* f(2m+1) = f(m) + 1 */
            p = p + q;
        } else {                     /* f(2m) = 1/(f(m)+1) */
            seq_t np = q;
            q = p + q;
            p = np;
        }
    }
    /* Step 2: find the Calkin-Wilf index of p/q.
       CW children: 2k -> p/(p+q), 2k+1 -> (p+q)/q. */
    int bits[130];
    int cnt = 0;
    while (!(p == q)) {
        if (p < q) {
            bits[cnt++] = 0;
            q = q - p;
        } else {
            bits[cnt++] = 1;
            p = p - q;
        }
    }
    seq_t m = 1;
    for (int i = cnt - 1; i >= 0; i--) {
        m = m * 2 + (seq_t)bits[i];
    }
    return m;
}
