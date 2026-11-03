/*
 * A349679  a(n) = n*(n+1)/2 - (n-2)*phi(n)/2 for n >= 2, with a(1)=1.
 * https://oeis.org/A349679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 - (n-2)*phi(n)/2 for n >= 2, with a(1) = 1
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 77 terms through n = 9559 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                 /* a(1) = 1 */

    seq_t n_seq = (seq_t)n;              /* promote index to seq_t */
    seq_t tri = n_seq * (n_seq + 1) / 2; /* triangular number */

    seq_t phi_val = totient(n);              /* Euler's totient of n */
    seq_t sub = ((seq_t)(n - 2) * phi_val) / 2;

    return tri - sub;
}
