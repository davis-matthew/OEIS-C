/*
 * A286725  Third column of triangle A286724: Lah[2,1](n+2, 2), n >= 0.
 * https://oeis.org/A286725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(n+2)!*binomial(n+2,2) Recurrence: a(0)=1; for
 *            n>=1, a(n) = a(n-1) * 2*(n+2)^2 / n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;                /* a(0) */
    if (n == 0) return res;

    for (idx_t k = 1; k <= n; ++k) {
        /* Multiply by 2*(k+2)^2 / k in an order that avoids overflow */
        res = res / (seq_t)k;              /* exact division */
        seq_t t = (seq_t)(k + 2);          /* k+2   */
        res = res * 2;                     /* *2    */
        res = res * t;                     /* *(k+2)*/
        res = res * t;                     /* *(k+2)*/
    }
    return res;
}
