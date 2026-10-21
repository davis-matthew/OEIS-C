/*
 * A280347  Numbers with 83 divisors.
 * https://oeis.org/A280347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime raised to the 82nd power
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Offset is 1: behaviour for n<1 is ignored */
    uint32_t p   = get_prime(n);         /* nth prime */
    seq_t   base = (seq_t)p;             /* Promote to seq_t */
    return seq_pow(base, (idx_t)82);     /* Compute base^82 */
}
