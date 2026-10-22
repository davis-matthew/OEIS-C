/*
 * A353830  The positions of nonzero digits in the balanced ternary expansions
 *          of n and a(n) are the same, and the k-th rightmost nonzero digit
 *          in a(n) equals the product of the k rightmost nonzero digits in n.
 * https://oeis.org/A353830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(3*n) = 3*a(n), a(3*n+1) = 3*a(n)+1. Computing a(n) directly
 *            from balanced ternary digits: Let the non‑zero digits of n (from
 *            least significant to most) be d0,d1,…. Let p_k = d0*d1*…*d_k.
 *            Then a(n) has non‑zero digits p0,p1,… in the same positions
 * Range:     i8 n <= 121, i16 n <= 6561, i32 n <= 6561, i64 n <= 6561,
 *            i128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for int64_t */

seq_t a(idx_t n)
{
    seq_t res   = 0;   /* the computed value of a(n)   */
    seq_t pow3  = 1;   /* running power of 3           */
    seq_t prod  = 1;   /* cumulative product of non‑zero digits of n */

    idx_t temp = n;    /* iterate over the digits of n */

    while (temp > 0) {
        int64_t r = (int64_t)(temp % 3);   /* initial remainder 0..2 */
        temp /= 3;
        if (r > 1) {                       /* convert 2 -> -1 with carry */
            r -= 3;        /* r becomes -1 */
            temp += 1;
        }

        if (r != 0) {                       /* non‑zero balanced ternary digit */
            prod *= (seq_t)r;                /* update cumulative product   */
            res  += prod * pow3;             /* add digit to result         */
        }

        pow3 *= 3;                          /* advance to next ternary place */
    }

    return res;
}
