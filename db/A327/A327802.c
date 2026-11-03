/*
 * A327802  Number of primes p such that n < p < (9/8) * n.
 * https://oeis.org/A327802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(ceil(9*n/8)-1) - pi(n)
 * Range:     u8 n <= 19673, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9655 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

/* Count primes p with n < p < (9/8) * n. */
seq_t a(idx_t n)
{
    /*  ceil(9*n/8) = (9*n + 7) / 8  because integer division floors.
        Subtract 1:  (9*n + 7)/8 – 1  equals (9*n - 1)/8 for all n. */
    idx_t m = (9 * n - 1) / 8;          /* upper bound for counting primes   */
    seq_t pi_m = prime_pi(m);          /* primes ≤ m */
    seq_t pi_n = prime_pi(n);          /* primes ≤ n */
    return pi_m - pi_n;                  /* primes in the strict open interval */
}
