/*
 * A319213  a(n) = phi(n^3 - 1)/3 where phi is A000010.
 * https://oeis.org/A319213
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n³ – 1) / 3, where phi is Euler's totient function
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 965 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided totient routine. */
#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 2: values for n < 2 are undefined. */
    if (n < 2)
        return 0;

    /* Work in seq_t for the exponentiation, but phi expects an idx_t. */
    seq_t sn = (seq_t)n;
    /* Compute n³ – 1 */
    seq_t tmp = sn * sn * sn;   /* n³  (seq_t arithmetic) */
    tmp = tmp - 1;              /* n³ – 1 */

    /* Cast back to idx_t (fits, as n³ ≤ 10⁴³ for all requested n). */
    idx_t val = (idx_t)tmp;

    /* phi(m) is guaranteed to be divisible by 3 for these arguments. */
    return totient(val) / 3;
}
