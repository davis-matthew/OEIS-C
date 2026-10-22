/*
 * A329379  a(n) = n/A093411(n), where A093411(n) is obtained by repeatedly
 *          dividing n by the largest factorial that divides it until an odd
 *          number is reached.
 * https://oeis.org/A329379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/A093411(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9191, u64 n <= 9191,
 *            u128 n <= 9191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093411(n) is obtained by repeatedly dividing n by the largest factorial
   that divides it until an odd number is reached.  a(n) = n divided by that
   odd remainder.  */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to 20! (sufficient for all n used in tests). */
    static seq_t fac[21];
    static bool init = false;
    if (!init) {
        fac[0] = (seq_t)1;
        for (int i = 1; i < 21; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;
        }
        init = true;
    }

    /* Work with a copy that will be reduced by factorials. */
    seq_t m = (seq_t)n;

    /* Repeat while the current value is even – any factorial >1 is even, so
       the process stops only when m is odd. */
    while ((m % (seq_t)2) == 0) {
        /* Find the largest factorial (≤20!) that divides m. */
        seq_t best = (seq_t)1;
        for (int k = 20; k >= 2; --k) {
            if (fac[k] <= m && (m % fac[k]) == 0) {
                best = fac[k];
                break;
            }
        }
        if (best == (seq_t)1) {
            /* No factorial >1 divides m – this should not happen for even m. */
            break;
        }
        m /= best;
    }

    /* The result is n divided by the final odd remainder m. */
    return ((seq_t)n) / m;
}
