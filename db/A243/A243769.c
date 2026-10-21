/*
 * A243769  a(n) = prime(n)^3 mod (n^2 + prime(n)^2).
 * https://oeis.org/A243769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^3 mod (n^2 + prime(n)^2)
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 6623, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, safe guard */

    seq_t p = (seq_t)get_prime(n);
    seq_t n_seq = (seq_t)n;

    seq_t denom = n_seq * n_seq + p * p;
    seq_t result = mod_pow(p, 3, denom);
    return result;
}
