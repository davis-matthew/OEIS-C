/*
 * A276812  Prime gap residues mod previous prime gap.
 * https://oeis.org/A276812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+2} - p_{n+1}) mod (p_{n+1} - p_n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t p_n   = (idx_t)get_prime(n);
    idx_t p_np1 = (idx_t)get_prime(n + 1);
    idx_t p_np2 = (idx_t)get_prime(n + 2);

    idx_t gap_prev = p_np1 - p_n;
    idx_t gap_next = p_np2 - p_np1;

    return (seq_t)(gap_next % gap_prev);
}
