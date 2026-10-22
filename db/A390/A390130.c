/*
 * A390130  Number of 4-partitions of the fan graph F_{1,n} where every
 *          triangle uses exactly 2 parts.
 * https://oeis.org/A390130
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A_{n-1,3} - 2*A_{n-1,2} + A_{n-1,1}) / 2 where A_{m,r} =
 *            F_m(r) + 2*F_{m-1}(r) + F_{m-2}(r), and F_k(r) is the k-th
 *            Fibonacci polynomial, F_0(r)=0, F_1(r)=1,
 *            F_{k+1}(r)=F_k(r)+r*F_{k-1}(r)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the sequence is not defined in OEIS */
    if (n < 6)
        return 0;

    idx_t m = n - 1;              /* m = n-1 */

    /* ---------- Standard Fibonacci for A_{m,1} = F_{m+2} ---------- */
    seq_t fib_prev2 = 0;           /* F_0   */
    seq_t fib_prev1 = 1;           /* F_1   */
    seq_t fib_curr = 0;
    for (idx_t i = 2; i <= m + 2; ++i) {
        fib_curr = fib_prev1 + fib_prev2;
        fib_prev2 = fib_prev1;
        fib_prev1 = fib_curr;
    }
    seq_t A1 = fib_curr;          /* F_{m+2} */

    /* ---------- Fibonacci polynomials for r = 2 and r = 3  ---------- */
    /* For r = 2 */
    seq_t f2_prev2 = 0;            /* F_0(2) */
    seq_t f2_prev1 = 1;            /* F_1(2) */
    seq_t f2_curr = 0;
    seq_t f2_m   = 0, f2_m1  = 0, f2_m2 = 0;

    /* For r = 3 */
    seq_t f3_prev2 = 0;            /* F_0(3) */
    seq_t f3_prev1 = 1;            /* F_1(3) */
    seq_t f3_curr = 0;
    seq_t f3_m   = 0, f3_m1  = 0, f3_m2 = 0;

    for (idx_t i = 2; i <= m; ++i) {
        /* Store values needed for A_{m,r} just before the update at i = m */
        if (i == m) {
            f2_m2 = f2_prev2;    /* F_{m-2}(2) */
            f2_m1 = f2_prev1;    /* F_{m-1}(2) */
            f3_m2 = f3_prev2;    /* F_{m-2}(3) */
            f3_m1 = f3_prev1;    /* F_{m-1}(3) */
        }

        /* r = 2 polynomial */
        f2_curr = f2_prev1 + 2 * f2_prev2;

        /* r = 3 polynomial */
        f3_curr = f3_prev1 + 3 * f3_prev2;

        /* Shift for next iteration */
        f2_prev2 = f2_prev1;
        f2_prev1 = f2_curr;
        f3_prev2 = f3_prev1;
        f3_prev1 = f3_curr;
    }
    /* After loop, f*_prev1 holds F_m(r) */
    f2_m = f2_prev1;
    f3_m = f3_prev1;

    /* Compute A_{m,2} and A_{m,3} */
    seq_t A2 = f2_m + 2 * f2_m1 + f2_m2;
    seq_t A3 = f3_m + 2 * f3_m1 + f3_m2;

    /* Final formula */
    seq_t temp = A3 - 2 * A2 + A1;
    return temp / 2;
}
