/*
 * A365266  a(n) = Product_{k=1..n} Gamma(6*k).
 * https://oeis.org/A365266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} Gamma(6*k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = ∏_{k=1}^{n} Γ(6k)  where Γ(m) = (m‑1)! for integer m. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = 1;
    seq_t fact   = 1;          /* factorial of current m */
    idx_t limit  = 6 * n - 1;  /* maximum m to reach 6n-1 */

    for (idx_t m = 1; m <= limit; ++m) {
        fact *= (seq_t)m;       /* update factorial */
        if (m % 6 == 5) {       /* m == 6k-1 */
            result *= fact;     /* multiply by that factorial */
        }
    }
    return result;
}
