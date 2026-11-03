/*
 * A337937  a(n) = Euler totient function phi = A000010 evaluated at N(n) =
 *          floor((3*n-1)/2) = A001651(n), for n >= 1.
 * https://oeis.org/A337937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(floor((3*n-1)/2))
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9347 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Compute N = floor((3*n-1)/2) safely in seq_t */
    seq_t N = (seq_t)3 * (seq_t)n;   /* multiply first */
    N -= 1;                         /* subtract one */
    N /= 2;                         /* integer division floors the result */
    return totient_seq(N);             /* Euler‑phi of N */
}
