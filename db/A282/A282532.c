/*
 * A282532  Position where the discrete difference of the Poissonian
 *          probability distribution function with mean n takes its lowest
 *          value. In case of a tie, pick the smallest value.
 * https://oeis.org/A282532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer k ≥ n such that P(n,k+1) – P(n,k)
 *            is minimal among all k. For a Poisson distribution, P(n,k+1) –
 *            P(n,k) = e^{-n} n^k/k! * (n/(k+1) – 1). The most negative
 *            difference occurs when the magnitude n^k/k! * (1 – n/(k+1)) is
 *            maximized for k ≥ n. This is obtained by iterating...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Ensure n >= 1 as per sequence definition */
    if (n < 1)
        return 0;          /* undefined behaviour outside offset */

    /* Start from k = n; it is the smallest possible position. */
    idx_t k = n;

    /* The condition for increasing the magnitude of the
       negative difference is:
           n * (k+2 - n) > (k+2) * (k+1 - n)
       The variables are kept in 64‑bit integers to avoid
       overflow for all practical n (≤ 10⁴ as required).          */
    while (true) {
        uint64_t kk = (uint64_t)k;
        uint64_t n64 = (uint64_t)n;
        uint64_t left  = n64 * (kk + 2 - n64);          /* n * (k+2 – n)   */
        uint64_t right = (kk + 2) * (kk + 1 - n64);     /* (k+2) * (k+1 – n) */

        if (left > right) {
            ++k;
        } else {
            break;
        }
    }

    return (seq_t)k;
}
