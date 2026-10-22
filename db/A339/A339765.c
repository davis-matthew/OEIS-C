/*
 * A339765  a(n) = 2*floor(n*phi) - 3*n, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A339765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*floor(n*phi) - 3*n where phi = (1+sqrt(5))/2 The floor
 *            is computed using the convergent 832040/514229, which is an
 *            excellent rational approximation to phi with error
 *            <1/(phi*F_29^2). For all n up to at least 10^9 the floor(n*phi)
 *            equals floor(n * 832040 / 514229)
 * Range:     i8 none, i16 n <= 2, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses only integer arithmetic (no floating point). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 832040 = F_30, 514229 = F_29 (Fibonacci numbers) */
    const seq_t fib_num = 832040;
    const seq_t fib_den = 514229;
    seq_t ns = (seq_t)n;                 /* cast index to the result type */
    seq_t floor_n_phi = (ns * fib_num) / fib_den;
    return 2 * floor_n_phi - 3 * ns;
}
