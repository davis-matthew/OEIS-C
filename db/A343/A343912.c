/*
 * A343912  a(n) = n - phi(n - phi(n)), where phi is the Euler totient
 *          function.
 * https://oeis.org/A343912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - phi(n - phi(n))
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)         /* a(0) is undefined for the sequence; return 0 for safety */
        return 0;

    /* phi(n) using the helper in phi.h */
    seq_t phi_n = totient((idx_t)n);

    /* compute n - phi(n) */
    seq_t m = (seq_t)n - phi_n;

    /* phi of that result */
    seq_t phi_m = totient((idx_t)m);

    /* final formula */
    return (seq_t)n - phi_m;
}
