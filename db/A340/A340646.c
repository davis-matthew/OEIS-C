/*
 * A340646  a(n) = (prime(n)^n) mod prime(n+1).
 * https://oeis.org/A340646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n mod prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0 as a placeholder. */
    if (n == 0)
        return 0;
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_np1 = (seq_t)get_prime(n + 1);
    return mod_pow(p_n, (uint64_t)n, p_np1);
}
