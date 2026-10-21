/*
 * A280350  Numbers with 97 divisors.
 * https://oeis.org/A280350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n)^96, where p_n is the n-th prime
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* According to the OEIS entry, a(n) are the 96th powers of the
       n-th primes (1-indexed).  For n == 0 the sequence is undefined;
       we return 0 in that case. */
    if (n == 0)
        return (seq_t)0;
    uint32_t p = get_prime(n);     /* nth prime, 1-indexed */
    seq_t base = (seq_t)p;
    seq_t exp  = (seq_t)96;
    return big_pow(base, exp);
}
