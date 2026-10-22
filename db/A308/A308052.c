/*
 * A308052  a(n) = nextprime(ceiling(n/2)-1), where nextprime(n) is the
 *          smallest prime > n.
 * https://oeis.org/A308052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nextprime(ceil(n/2)-1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* ceil(n/2)-1 equals (n-1)/2 for unsigned n */
    idx_t k = (n - 1) / 2;
    uint64_t p = next_prime(k);
    return (seq_t)p;
}
