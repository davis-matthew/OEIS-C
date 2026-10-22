/*
 * A353946  a(n) = (pi(2n-1) - pi(n-1))^pi(n) for n > 1, a(1) = 0.
 * https://oeis.org/A353946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (pi(2n-1) - pi(n-1))^pi(n) for n>1, a(1) = 0
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n <= 1)
        return 0;

    /* π(n) */
    seq_t pi_n = compute_pi(n);

    /* π(n-1) – careful with subtraction on idx_t */
    seq_t pi_n_minus_1 = compute_pi(n - (idx_t)1);

    /* π(2n-1) */
    idx_t two_n_minus_1 = n * (idx_t)2 - (idx_t)1;
    seq_t pi_2n_minus_1 = compute_pi(two_n_minus_1);

    /* Base for exponentiation */
    seq_t base = pi_2n_minus_1 - pi_n_minus_1;

    /* Result: base^pi_n */
    return big_pow(base, pi_n);
}
