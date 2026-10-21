/*
 * A267144  Octagonal numbers with prime indices.
 * https://oeis.org/A267144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*(3*prime(n)-2)
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* offset 1: return 0 for n < 1 (behavior below the offset is unspecified) */
    if (n < 1) {
        return (seq_t)0;
    }

    /* get nth prime (1‑based) as a 32‑bit unsigned integer */
    uint32_t p = get_prime(n);

    /* cast to seq_t and compute the formula */
    seq_t ps   = (seq_t)p;
    seq_t mult = ps * 3;          /* 3*prime */
    seq_t res  = ps * (mult - (seq_t)2);
    return res;
}
