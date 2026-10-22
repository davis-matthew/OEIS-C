/*
 * A341545  a(n) = sqrt( Product_{j=1..n} Product_{k=1..6}
 *          (4*sin((2*j-1)*Pi/(2*n))^2 + 4*sin((2*k-1)*Pi/6)^2) ).
 * https://oeis.org/A341545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A341545:
   a(n) = sqrt(∏_{j=1..n} ∏_{k=1..6} (4*sin((2j-1)π/(2n))^2 + 4*sin((2k-1)π/6)^2))

   Derived integer formula:
   a(n) =
       4  * T_n(√2)^2 * L_n^4          if n is even
       200 * (T_n(√2)/√2)^2 * F_n^4   if n is odd

   where F_n and L_n are Fibonacci and Lucas numbers,
   and T_n(√2) are Chebyshev polynomials of first kind evaluated at √2.
   For even n:  T_n(√2) is integer; for odd n:  T_n(√2) = b_n·√2.

   The recurrence for the integer part (a_n) and √2 coefficient (b_n) of T_n(√2):
       a_0 = 1,  b_0 = 0
       a_1 = 0,  b_1 = 1
       a_{k+1} = 4*b_k - a_{k-1}
       b_{k+1} = 2*a_k - b_{k-1}
   For Fibonacci/Lucas:
       F_0 = 0, L_0 = 2
       F_1 = 1, L_1 = 1
       F_{k+1} = F_k + F_{k-1}
       L_{k+1} = L_k + L_{k-1}
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Offset = 1, a(1) is the first term. For n < 1 result is arbitrary. */
    if (n < 1)
        return 0;

    /* ---------- Fibonacci and Lucas numbers ---------- */
    seq_t F_prev = 0, F_cur = 1;   /* F_0, F_1 */
    seq_t L_prev = 2, L_cur = 1;   /* L_0, L_1 */

    if (n >= 2) {
        idx_t i;
        for (i = 2; i <= n; ++i) {
            seq_t F_next = F_prev + F_cur;
            F_prev = F_cur;
            F_cur  = F_next;

            seq_t L_next = L_prev + L_cur;
            L_prev = L_cur;
            L_cur  = L_next;
        }
    }
    /* At this point: F_cur = F_n, L_cur = L_n. */

    /* ---------- Chebyshev T_n(√2) coefficients ---------- */
    seq_t a_prev2 = 1, a_prev1 = 0; /* a_0, a_1 */
    seq_t b_prev2 = 0, b_prev1 = 1; /* b_0, b_1 */

    if (n >= 2) {
        idx_t i;
        for (i = 2; i <= n; ++i) {
            seq_t a_next = (seq_t)4 * b_prev1 - a_prev2;
            seq_t b_next = (seq_t)2 * a_prev1 - b_prev2;

            a_prev2 = a_prev1; a_prev1 = a_next;
            b_prev2 = b_prev1; b_prev1 = b_next;
        }
    }
    /* After loop: a_prev1 = a_n, b_prev1 = b_n. */

    /* ---------- Compute the result ---------- */
    if ((n & 1) == 0) { /* even n */
        seq_t t_n = a_prev1;            /* integer T_n(√2) */
        seq_t temp = t_n * t_n;         /* t_n^2 */

        seq_t l2 = L_cur * L_cur;       /* L_n^2 */
        l2 = l2 * l2;                   /* L_n^4 */

        temp = temp * l2;               /* t_n^2 * L_n^4 */
        temp = temp * 4;                /* final factor 4 */
        return temp;
    } else { /* odd n */
        seq_t b_n = b_prev1;            /* coefficient of √2 in T_n(√2) */
        seq_t temp = b_n * b_n;         /* b_n^2 */

        seq_t f2 = F_cur * F_cur;       /* F_n^2 */
        f2 = f2 * f2;                   /* F_n^4 */

        temp = temp * f2;               /* b_n^2 * F_n^4 */
        temp = temp * 200;              /* final factor 200 */
        return temp;
    }
}
