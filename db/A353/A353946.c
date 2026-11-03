/*
 * A353946  a(n) = (pi(2n-1) - pi(n-1))^pi(n) for n > 1, a(1) = 0.
 * https://oeis.org/A353946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (pi(2n-1) - pi(n-1))^pi(n) for n>1, a(1) = 0
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   ipow, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n <= 1)
        return 0;

    /* π(n) */
    seq_t pi_n = prime_pi(n);

    /* π(n-1) – careful with subtraction on idx_t */
    seq_t pi_n_minus_1 = prime_pi(n - (idx_t)1);

    /* π(2n-1) */
    idx_t two_n_minus_1 = n * (idx_t)2 - (idx_t)1;
    seq_t pi_2n_minus_1 = prime_pi(two_n_minus_1);

    /* Base for exponentiation */
    seq_t base = pi_2n_minus_1 - pi_n_minus_1;

    /* Result: base^pi_n */
    return ipow(base, pi_n);
}
