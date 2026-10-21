/*
 * A297170  a(n) = gcd(phi(n), sigma(n)-n).
 * https://oeis.org/A297170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(phi(n), sigma(n)-n)
 * Range:     u8 n <= 751, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A297170:
   a(n) = gcd(ϕ(n), σ(n) - n)  (offset 1) */
  
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t phi_n   = phi(n);
    seq_t sigma_n = sigma_of(n);
    seq_t diff    = sigma_n - (seq_t)n;
    return gcd(phi_n, diff);
}
