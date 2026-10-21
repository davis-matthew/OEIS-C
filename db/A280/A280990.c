/*
 * A280990  Least prime p such that n divides phi(p*n).
 * https://oeis.org/A280990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Least prime p such that n divides φ(p · n)
 * Range:     u8 n <= 76, u16 n <= 1690, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm simply searches primes in increasing order until the condition is met. */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, but safeguard */

    /* compute φ(n) */
    seq_t phi_seq = calc_phi((seq_t)n);
    idx_t phi_val = (idx_t)phi_seq;   /* φ(n) fits in idx_t for the input ranges */
    idx_t phi_mod = phi_val % n;      /* φ(n) mod n  (equals φ(n) except for n=1) */

    /* iterate over primes */
    for (idx_t p = 2; ; ++p) {
        if (!is_prime_big((seq_t)p)) continue;

        idx_t mult;
        if (n % p == 0) {
            /* φ(p·n) = p · φ(n)  (mod n) */
            mult = ((p % n) * phi_mod) % n;
        } else {
            /* φ(p·n) = (p-1) · φ(n) (mod n) */
            mult = (((p - 1) % n) * phi_mod) % n;
        }

        if (mult == 0) return (seq_t)p;
    }
}
