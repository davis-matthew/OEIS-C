/*
 * A268342  Number of edges in the unitary addition Cayley graph Gn.
 * https://oeis.org/A268342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*phi(n) if n even, (n-1)*phi(n)/2 if n odd
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t phi_n = totient(n);
    if ((n & 1) == 0) {
        /* n is even: a(n) = n * phi(n) */
        return (seq_t)n * phi_n;
    } else {
        /* n is odd: a(n) = (n-1) * phi(n) / 2 */
        return ((seq_t)(n - 1) * phi_n) / 2;
    }
}
