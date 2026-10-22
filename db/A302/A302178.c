/*
 * A302178  The number of 3D walks of semilength n in a quadrant returning to
 *          the origin.
 * https://oeis.org/A302178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A302178: number of 3‑D quadrant walks of semilength n returning to the origin
 *
 *  Recurrence (valid for all n ≥ 3):
 *      (n+1)(n+2)² a(n) + 2*(-29n³-39n²+8n+6) a(n-1)
 *      + 36(4n-1)(2n-3)(3n-2) a(n-2)
 *      - 648(2n-5)(n-2)(2n-3) a(n-3)   = 0
 *
 *  Initial values:
 *      a(0)=1, a(1)=4, a(2)=40
 *
 *  The implementation uses 128‑bit arithmetic for the intermediate
 *  products so that 64‑bit builds are accurate up to the limits tested
 *  (n ≈ 10 000).  If seq_t is an arbitrary‑precision class, the
 *  conversion from __uint128_t to seq_t works via the class's
 *  constructor.
 */

#include <stddef.h>   /* for size_t, required by __uint128_t macros */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)4;
    if (n == 2ULL) return (seq_t)40;

    seq_t a_prev3 = (seq_t)1;   /* a(n-3)   */
    seq_t a_prev2 = (seq_t)4;   /* a(n-2)   */
    seq_t a_prev1 = (seq_t)40;  /* a(n-1)   */
    seq_t a_curr  = (seq_t)0;

    for (idx_t k = 3ULL; k <= n; ++k)
    {
        /* coefficients from the recurrence (sign has been taken into
         * account by the +/- in the numerator construction below */
        unsigned __int128 k3 = (unsigned __int128)k * k * k;

        unsigned __int128 t1 = (unsigned __int128)2 *
                               ((unsigned __int128)29 * k3 +
                                (unsigned __int128)39 * (unsigned __int128)k * k -
                                (unsigned __int128)8 * k -
                                (unsigned __int128)6);

        unsigned __int128 t2 = (unsigned __int128)36 *
                               (4 * k - 1) *
                               (2 * k - 3) *
                               (3 * k - 2);

        unsigned __int128 t3 = (unsigned __int128)648 *
                               (2 * k - 5) *
                               (k - 2) *
                               (2 * k - 3);

        /* numerator (all terms are now 128‑bit) */
        unsigned __int128 num = (unsigned __int128)a_prev1 * t1
                              - (unsigned __int128)a_prev2 * t2
                              + (unsigned __int128)a_prev3 * t3;

        /* denominator (n+1)(n+2)² */
        unsigned __int128 denom = (unsigned __int128)(k + 1ULL) *
                                  (unsigned __int128)(k + 2ULL) *
                                  (unsigned __int128)(k + 2ULL);

        a_curr = (seq_t)(num / denom);

        /* shift terms for the next iteration */
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
