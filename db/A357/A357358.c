/*
 * A357358  Length of the longest induced cycle in the n X n torus grid graph.
 * https://oeis.org/A357358
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(2*n^2/3) - f(n) (n >= 3) where f(n) depends on n
 *            modulo 6: n%6==0 : f(n) = 2*n/3 n%6==1 : f(n) = 2*(n-1)/3 n%6==2
 *            : f(n) = (n-2)/3 n%6==3 : f(n) = n-3 n%6==4 : f(n) = n-2 n%6==5
 *            : f(n) = n-4
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;               /* undefined for n < 3, safe fallback */
    /* use seq_t for arithmetic that may overflow in idx_t for large n */
    seq_t nn = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t base = (nn * 2) / 3;             /* floor(2*n^2/3) */
    idx_t r = n % 6;
    seq_t diff;
    switch (r) {
    case 0: diff = ((seq_t)2 * n) / 3;          break;
    case 1: diff = ((seq_t)2 * (n - 1)) / 3;    break;
    case 2: diff = ((seq_t)(n - 2)) / 3;          break;
    case 3: diff = (seq_t)(n - 3);                 break;
    case 4: diff = (seq_t)(n - 2);                 break;
    case 5: diff = (seq_t)(n - 4);                 break;
    default: diff = 0;                              /* unreachable */
    }
    return base - diff;
}
