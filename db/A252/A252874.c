/*
 * A252874  Number of n X 6 nonnegative integer arrays with upper left 0 and
 *          lower right n+6-4 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/11404800)*n^12 + (1/211200)*n^11 + (37/345600)*n^10 +
 *            (143/103680)*n^9 + (27977/2419200)*n^8 + (83399/1209600)*n^7 +
 *            (318253/1036800)*n^6 + (6655/6912)*n^5 + (17647/9600)*n^4 +
 *            (350243/129600)*n^3 + (95119/277200)*n^2 + (5297/6930)*n + 3.
 *            Implementation: integer linear combination of bi...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 151, u128 n <= 210,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Integer coefficients of the binomial expansion
       a(n) = Σ coeff[k] * binomial(n - k + 12, 12),  k = 1 .. 13 */
    static const int coeff[13] = {
        10, -10, 169, -556, 1402, -2404, 2895,
        -2526, 1599, -716, 214, -38, 3
    };

    /* Use a wider type for intermediate sums to avoid overflow
       on 64‑bit builds.  For arbitrary‑precision builds the cast
       to seq_t is handled by the library's conversion operator. */
    __int128 pos = 0;
    __int128 neg = 0;

    for (int k = 1; k <= 13; ++k) {
        if (n < (idx_t)k)          /* binomial would be zero */
            continue;

        idx_t a_val = n - (idx_t)k + 12;      /* a = n - k + 12  */
        seq_t term = binomial(a_val, 12);     /* binomial(a_val,12) */
        int c = coeff[k - 1];

        if (c > 0) {
            pos += (__int128)term * c;
        } else {
            neg += (__int128)term * (-c);
        }
    }

    __int128 result = pos - neg;
    return (seq_t)result;
}
