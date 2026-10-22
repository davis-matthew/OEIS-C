/*
 * A376252  Concatenated (n+1)||n modulo n.
 * https://oeis.org/A376252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^A055642(n) mod n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   digit_count, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_count.h"
#include "../utils/mod_pow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, not expected by the offset */
    seq_t mod  = (seq_t)n;                /* modulo n */
    seq_t base = 10 % mod;                /* 10 modulo n */
    unsigned d = (unsigned)digit_count(n);/* number of decimal digits of n */
    return mod_pow(base, d, mod);         /* 10^d mod n */
}
