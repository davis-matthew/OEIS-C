/*
 * A245035  a(n) = (prime(n) - 7)^2 * (4*prime(n) - 1).
 * https://oeis.org/A245035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 7)^2 * (4*prime(n) - 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 172, u64 n <= 6320,
 *            u128 n <= 6320, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined only for n >= 1 */

    /* Retrieve the n‑th prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* Perform intermediate arithmetic in signed 64‑bit type
       to avoid unsigned wrap‑around for the (p-7) subtraction.  */
    int64_t d    = (int64_t)p - 7;          /* p - 7 (may be negative) */
    int64_t d2   = d * d;                   /* (p-7)^2  – non‑negative  */
    int64_t mult = (int64_t)4 * (int64_t)p - 1;
    int64_t res  = d2 * mult;               /* final value   */

    /* Result fits into seq_t whenever it fits; otherwise the
       cast simply wraps modulo 2^width(seq_t). */
    return (seq_t)res;
}
