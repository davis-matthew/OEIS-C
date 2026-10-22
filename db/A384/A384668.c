/*
 * A384668  a(n) = 12 * (5*n+2)! / ((3*n+1)! * (2*n+2)!).
 * https://oeis.org/A384668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * (5*n+2)! / ((3*n+1)! * (2*n+2)!) Equivalent: a(n) =
 *            6 * binom(5*n+2, 3*n+1) / (n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>           /* for idx_t/numeric limits (supplied by build) */

seq_t a(idx_t n)
{
    /* ------------- arithmetic using a wider type if the compiler provides it ------------- */
#ifdef __SIZEOF_INT128__          /* available on GCC/Clang for 128‑bit arithmetic      */
    /* Branch only used when seq_t is a built‑in type of ≤ 8 bytes.
       For arbitrary‑precision seq_t the code below will not be executed.              */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 acc = 1;     /* accumulator wide enough for all intermediates   */
        const idx_t N = 5 * n + 2;      /* upper index of the factorial                    */
        const idx_t K = 3 * n + 1;      /* number of binomial terms                        */

        for (idx_t i = 1; i <= K; ++i) {
            acc = acc * (N - K + i);    /* multiply by the current numerator term          */
            acc = acc / i;              /* exact division by the current denominator term  */
        }
        acc = acc * 6;                  /* multiply by the outer factor 6                  */
        acc = acc / (n + 1);            /* final division by (n+1)                         */
        return (seq_t)acc;              /* cast back to the native sequence type            */
    }
#endif

    /* ------------- generic implementation for arbitrary‑precision seq_t ------------- */
    /* This branch is used either when seq_t is larger than 8 bytes, or when the
       compiler has no __int128 support.  It relies only on operations defined for
       seq_t, which work correctly for any size (built‑in types or big‑int classes). */
    {
        seq_t res = 1;
        const idx_t N = 5 * n + 2;
        const idx_t K = 3 * n + 1;

        for (idx_t i = 1; i <= K; ++i) {
            res = res * (seq_t)(N - K + i);
            res = res / (seq_t)i;
        }
        res = res * 6;
        res = res / (seq_t)(n + 1);
        return res;
    }
}
