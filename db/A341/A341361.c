/*
 * A341361  a(n) is the smallest abundant number of the form 2^e * prime(n).
 * https://oeis.org/A341361
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * 2^⌊log₂(prime(n)+1)⌋ (i.e., the smallest
 *            integer of the form 2^e * prime(n) that is abundant)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"              /* declares uint32_t get_prime(idx_t k); */
#include "../utils/floor_log2_uint64.h"      /* declares unsigned floor_log2_uint64(idx_t v); */

seq_t a(idx_t n)
{
    if (n < 2)                   /* sequence is undefined for n < 2 */
        return (seq_t)0;

    /* retrieve the n-th prime (1‑based) */
    uint32_t p32 = get_prime(n);
    seq_t    p   = (seq_t)p32;

    /* exponent e = floor(log2(p + 1)) */
    unsigned e = floor_log2_uint64((idx_t)p32 + 1);

    /* value is p * 2^e */
    seq_t pow2 = ((seq_t)1) << e;
    return p * pow2;
}
