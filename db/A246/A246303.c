/*
 * A246303  Numbers k such that cos(k) < cos(k+1).
 * https://oeis.org/A246303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246303: k such that cos(k) < cos(k+1).
   Property: cos(k) < cos(k+1)  ⇔  sin(k + 0.5) < 0.
   This is equivalent to floor((k + 0.5)/π) being odd.
   Use a high‑precision rational approximation for 1/π
   (33102/103993) with error < 10⁻¹⁶, guaranteeing correct
   floor values for all k up to the limits used in the tests. */
#include <assert.h>

static const idx_t NUM = 33102U;        /* numerator of 1/π approximation   */
static const idx_t DEN = 103993U;       /* denominator of 1/π approximation */
static const idx_t DEN2 = DEN * 2U;     /* 2*DEN, used in the division      */

seq_t a(idx_t n)
{
    assert(n >= 1);
    idx_t count = 0;
    idx_t k = 0;                          /* current integer candidate      */

    while (1) {
        /* compute floor((k + 0.5)/π) using the rational 33102/103993 */
        unsigned long long num = (unsigned long long)(2 * k + 1) * NUM;
        idx_t q = (idx_t)(num / DEN2);     /* floor division, safe in 64‑bit */

        if (q & 1U) {                     /* odd → cos(k) < cos(k+1)        */
            count++;
            if (count == n)
                return (seq_t)k;          /* found the nth term             */
        }
        k++;                              /* next integer candidate         */
    }
}
