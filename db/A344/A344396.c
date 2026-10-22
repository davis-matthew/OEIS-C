/*
 * A344396  a(n) = binomial(2*n + 1, n)*hypergeom([-(n + 1)/2, -n/2], [n + 2],
 *          4).
 * https://oeis.org/A344396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* P‑recursive recurrence (see OEIS entry):
   3*(13*n-4)*(3*n+2)*(3*n+1)*(n+1)*a(n) =
     2*(2*n+1)*(455*n^3+315*n^2-44*n-24)*a(n-1)          + 
     36*(13*n+9)*(2*n+1)*(2*n-1)*n*a(n-2)
   with initial values a(0)=1, a(1)=5
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)1; /* a(n-2) */
    seq_t a1 = (seq_t)5; /* a(n-1) */
    seq_t ai;            /* a(n) */

    /* Iterate from n = 2 up to the requested value */
    for (idx_t i = 2; i <= n; ++i) {
        /* Mixed implementation: use 128‑bit intermediate for the
           fixed‑width integer builds, otherwise use seq_t operators
           which are defined for arbitrary‑precision types.            */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 nn = (unsigned __int128)i;

            unsigned __int128 t1 =
                455u * nn * nn * nn       /* 455*n^3  */
              + 315u * nn * nn            /* 315*n^2 */
              - 44u  * nn                 /* -44*n   */
              - 24u;                      /* -24     */

            unsigned __int128 coeff1 =
                2u * (2u * nn + 1u) * t1;

            unsigned __int128 coeff2 =
                36u * (13u * nn + 9u)
                * (2u * nn + 1u)
                * (2u * nn - 1u)
                * nn;

            unsigned __int128 denom =
                3u * (13u * nn - 4u)
                * (3u * nn + 2u)
                * (3u * nn + 1u)
                * (nn + 1u);

            unsigned __int128 num =
                coeff1 * (unsigned __int128)a1
              + coeff2 * (unsigned __int128)a0;

            ai = (seq_t)(num / denom);
        } else {
            seq_t nn = (seq_t)i;

            seq_t t1 =
                455 * nn * nn * nn
              + 315 * nn * nn
              - 44  * nn
              - 24;

            seq_t coeff1 = 2 * (2 * nn + (seq_t)1) * t1;

            seq_t coeff2 =
                36 * (13 * nn + (seq_t)9)
                * (2 * nn + (seq_t)1)
                * (2 * nn - (seq_t)1)
                * nn;

            seq_t denom =
                3 * (13 * nn - (seq_t)4)
                * (3 * nn + (seq_t)2)
                * (3 * nn + (seq_t)1)
                * (nn + (seq_t)1);

            seq_t num = coeff1 * a1 + coeff2 * a0;

            ai = num / denom;
        }

        /* Shift for next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
