/*
 * A373099  Last digit of n*2^n + 1.
 * https://oeis.org/A373099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n*2^n + 1) mod 10
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* compute 2^n mod 10 */
    seq_t pow2mod = mod_pow((seq_t)2, n, (seq_t)10);

    /* only n mod 10 matters for the product modulo 10 */
    seq_t nmod = (seq_t)(n % (idx_t)10);

    /* compute (n*2^n) mod 10 safely */
    seq_t prod = nmod * pow2mod;

    /* add 1 and reduce modulo 10 */
    seq_t res = (prod + (seq_t)1) % (seq_t)10;

    return res;
}
