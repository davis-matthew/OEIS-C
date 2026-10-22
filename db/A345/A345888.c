/*
 * A345888  a(n) = n + (n - 1) * pi(n).
 * https://oeis.org/A345888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n-1)*pi(n)
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    seq_t pi_n = compute_pi(n);
    return (seq_t)n + ((seq_t)n - 1) * pi_n;
}
