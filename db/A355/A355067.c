/*
 * A355067  a(n) is the failed skew zero forcing number of P^3_n.
 * https://oeis.org/A355067
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor((n-1)/4) + cos((n*Pi)/2) for n > 7 * a(3)=0,
 *            a(4)=1, a(5)=3, a(6)=3, a(7)=4
 * Range:     u8 n <= 273, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – any value is fine. */
    if (n < 3)
        return 0;

    /* Explicit values for the first five terms (n = 3 … 7). */
    static const seq_t small[5] = {0, 1, 3, 3, 4};
    if (n <= 7)
        return small[n - 3];

    /* For n > 7 use the closed‑form formula.  */
    seq_t term = 3 * ((seq_t)(n - 1) / 4);   /* 3*floor((n-1)/4) */
    seq_t result = term;

    /* Add the cos((n*Pi)/2) contribution,
     * which is 1 for n ≡ 0 (mod 4), -1 for n ≡ 2 (mod 4),
     * and 0 otherwise.  */
    switch (n & 3U) {            /* equivalent to n % 4 */
        case 0:  result += 1; break;
        case 2:  result -= 1; break;
        default:                /* cases 1 and 3: no change */
            break;
    }
    return result;
}
