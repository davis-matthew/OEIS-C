/*
 * A290443  a(n) = Catalan(n)*Motzkin(n-1).
 * https://oeis.org/A290443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n)*Motzkin(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* Undefined for n<1 */

    /* Compute Motzkin number M(n-1) using recurrence */
    idx_t k = n - 1;                    /* index of Motzkin number */

    seq_t M0 = 1;                       /* M(0) */
    if (k == 0) {
        return catalan(n);
    }
    seq_t M1 = 1;                       /* M(1) */
    if (k == 1) {
        return catalan(n) * M1;
    }

    seq_t Mk = 1;
    for (idx_t i = 2; i <= k; ++i) {
        /* M(i) = ((2*i+1)*M(i-1) + (3*i-3)*M(i-2)) / (i+2) */
        seq_t term1 = ((seq_t)(2 * i + 1)) * M1;
        seq_t term2 = ((seq_t)(3 * i - 3)) * M0;
        Mk = (term1 + term2) / ((seq_t)(i + 2));
        M0 = M1;          /* shift for next iteration */
        M1 = Mk;
    }

    return catalan(n) * Mk;
}
