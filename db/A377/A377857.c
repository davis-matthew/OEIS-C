/*
 * A377857  Number of subwords of the form UUUD in nondecreasing Dyck paths of
 *          length 2n.
 * https://oeis.org/A377857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(2*n-5) - L(2*n-6) (using Fibonacci F(_) and Lucas
 *            L(_))
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 1;   /* a(3) = 1 by definition */

    /* For n >= 4 the indices are non‑negative. */
    idx_t k = 2*n - 5;           /* k = 2*n-5 >= 3   */
    seq_t fk   = fib(k);         /* F(k)                     */
    seq_t fkm2 = fib(k-2);       /* F(k-2) = F(2*n-7)        */

    /* L(2*n-6) = F(2*n-7) + F(2*n-5) = fkm2 + fk   */
    seq_t l = fkm2 + fk;

    return (seq_t)n * fk - l;
}
