/*
 * A259923  a(n) = prime(n)^pi(n).
 * https://oeis.org/A259923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^pi(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t base   = (seq_t)get_prime(n);
    seq_t exp    = compute_pi(n);
    return big_pow(base, exp);
}
