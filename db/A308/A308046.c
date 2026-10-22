/*
 * A308046  a(n) = 2*nextprime(n - 1) - 2*n, where nextprime(n) is the
 *          smallest prime > n.
 * https://oeis.org/A308046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*nextprime(n - 1) - 2*n
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 in usage, but we guard against underflow */
    if (n == 0) return (seq_t)0;
    uint64_t nm1 = n - 1;              /* smallest integer > n-1 is n */
    uint64_t p = next_prime(nm1);      /* smallest prime > n-1 */
    seq_t primes_val = (seq_t)p;
    seq_t n_val      = (seq_t)n;
    return (primes_val << 1) - (n_val << 1);   /* 2*p - 2*n */
}
