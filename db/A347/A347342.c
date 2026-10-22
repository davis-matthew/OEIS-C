/*
 * A347342  a(n) = prime(n) mod floor(prime(n) / n).
 * https://oeis.org/A347342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) mod floor(prime(n) / n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* prime(n) fits in 32 bits for all n of interest (n <= 10000 → prime ~ 1e5) */
    uint32_t p32 = get_prime(n);
    uint64_t p64  = p32;                 /* promote to avoid overflow in division */
    uint64_t div  = p64 / (uint64_t)n;   /* floor(prime(n) / n) */
    if (div == 0) return (seq_t)p64;     /* safety net – should not occur for n>=1 */
    return (seq_t)(p64 % div);
}
