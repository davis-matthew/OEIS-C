/*
 * A345469  a(n) is the least k such that A345468(k) = 2*n-1.
 * https://oeis.org/A345469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that (2*n-1)^2 + 4*k is prime and
 *            (2*m-1)^2 + 4*k is composite for all 1 ≤ m < n
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t odd_n = (seq_t)(2 * n - 1);           /* 2*n-1 as seq_t */
    seq_t odd_n_sq = odd_n * odd_n;            /* (2*n-1)^2 */

    for (idx_t k = 1; ; ++k) {
        seq_t val_n = odd_n_sq + (seq_t)4 * k;
        if (!isPrimeGeneric(val_n))
            continue;

        bool ok = true;
        for (idx_t m = 1; m < n; ++m) {
            seq_t odd_m = (seq_t)(2 * m - 1);
            seq_t val_m = odd_m * odd_m + (seq_t)4 * k;
            if (isPrimeGeneric(val_m)) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)k;
    }
}
