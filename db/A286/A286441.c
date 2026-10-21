/*
 * A286441  Number of ways to tile an n X n X n triangular area with six 2 X 2
 *          X 2 triangular tiles and an appropriate number (= n^2-24) of 1 X 1
 *          X 1 tiles.
 * https://oeis.org/A286441
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 - 18*n^11 + 3*n^10 + 1710*n^9 - 7175*n^8 -
 *            60078*n^7 + 401649*n^6 + 884466*n^5 - 9521846*n^4 - 3238224*n^3
 *            + 107453448*n^2 - 25651296*n - 483140880) / 720 for n >= 7; a(5)
 *            = 0, a(6) = 219. The function works for all n >= 5
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 13, u64 n <= 71, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Use 128‑bit intermediate type if available. */
#if defined(__SIZEOF_INT128__)
#   define NUM_T __uint128_t
#else
#   define NUM_T seq_t
#endif

seq_t a(idx_t n) {
    /* Handle the offset and small special cases */
    if (n == 5) return (seq_t)0;
    if (n == 6) return (seq_t)219;
    if (n < 5)  return (seq_t)0;   /* undefined below the offset */

    /* Coefficients of the 12th‑degree polynomial (from n^11 down to n^0). */
    static const int coeffs[12] = {
        -18,          /* n^11 */
          3,          /* n^10 */
        1710,          /* n^9 */
       -7175,          /* n^8 */
      -60078,          /* n^7 */
      401649,          /* n^6 */
       884466,         /* n^5 */
     -9521846,         /* n^4 */
     -3238224,         /* n^3 */
    107453448,         /* n^2 */
   -25651296,          /* n^1 */
   -483140880          /* n^0 */
    };

    NUM_T num = (NUM_T)1;               /* start with leading coefficient of n^12 (which is 1) */
    NUM_T nn  = (NUM_T)n;               /* cast n to the intermediate type */

    for (int i = 0; i < 12; ++i) {
        /* Multiply by n */
        num = num * nn;

        /* Add the current coefficient */
        int c = coeffs[i];
        if (c < 0) {
            num -= (NUM_T)(-c);
        } else {
            num += (NUM_T)c;
        }
    }

    /* Final division by 720 (exact for all n >= 7). */
    num /= (NUM_T)720;

    return (seq_t)num;   /* cast back to seq_t.  For 64‑bit builds this is safe
                           as the caller only tests values that fit in 64 bits. */
}
