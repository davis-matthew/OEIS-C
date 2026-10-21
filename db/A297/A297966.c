/*
 * A297966  a(n) = 3^n mod prime(n).
 * https://oeis.org/A297966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as placeholder */
    seq_t p = (seq_t)get_prime(n); /* nth prime, 1‑indexed */
    return mod_pow((seq_t)3, n, p);
}
