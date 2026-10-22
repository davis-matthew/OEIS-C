/*
 * A329584  phi(A327922(n))/4, for n >= 1, with phi = A000010 (Euler's
 *          totient).
 * https://oeis.org/A329584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(m_n)/4 where m_n is the n‑th odd integer ≥3 with φ(m_n)
 *            divisible by 4
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = A000010(A327922(n))/4 for n >= 1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1; guard against 0 for robustness */
    if (n == 0) return 0;

    idx_t count = 0;          /* number of qualifying odd integers found so far */
    idx_t m = 3;              /* start from the first odd integer >= 3 */

    for (;;) {
        seq_t phi_m = calc_phi((seq_t)m);

        /* test whether φ(m) is a multiple of 4 */
        if ((phi_m % 4) == 0u) {
            ++count;
            if (count == n)
                return phi_m / 4;
        }
        m += 2;  /* next odd integer */
    }
}
