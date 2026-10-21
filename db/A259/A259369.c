/*
 * A259369  a(n) = 1 + sigma(n)^3 + sigma(n)^6.
 * https://oeis.org/A259369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)^3 + sigma(n)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 539,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"   /* sigma_of(idx_t n) -> seq_t */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    seq_t s  = sigma_of(n);              /* sum of divisors of n */
    seq_t c3 = seq_pow(s, (idx_t)3);     /* s^3   */
    seq_t c6 = seq_pow(s, (idx_t)6);     /* s^6   */
    return ((seq_t)1) + c3 + c6;         /* 1 + s^3 + s^6 */
}
