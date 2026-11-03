/*
 * A362719  Number of numbers k, 1 <= k <= n, such that phi(k) = phi(n-k+1).
 * https://oeis.org/A362719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} [phi(k) = phi(n-k+1)]
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Validated: partial, pass, 95 terms through n = 95 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* For safety, ignore n = 0 (undefined for the sequence). */
    if (n == 0) return (seq_t)0;

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t phik = totient(k);                 /* phi(k) */
        seq_t phin = totient(n - k + 1);         /* phi(n-k+1) */
        if (phik == phin) {
            ++count;
        }
    }
    return count;
}
