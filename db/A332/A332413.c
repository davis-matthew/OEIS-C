/*
 * A332413  a(n) is the imaginary part of f(n) = Sum_{d_k > 0} 3^k * i^(d_k-1)
 *          where Sum_{k >= 0} 5^k * d_k is the base 5 representation of n and
 *          i denotes the imaginary unit. Sequence A332412 gives real parts.
 * https://oeis.org/A332413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 6249, i16 n <= 8184, i32 n <= 8184, i64 n <= 8184,
 *            i128 n <= 8184, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(5*n+r) = 3*a(n) + c[r] where c[0..4] = {0,0,1,0,-1} */
/* a(0) = 0 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* recursive evaluation using the recurrence */
    seq_t sub = a(n / 5);
    seq_t res = sub * 3;

    switch (n % 5)
    {
        case 2:
            res += 1;
            break;
        case 4:
            res -= 1;
            break;
        default:
            break;
    }
    return res;
}
