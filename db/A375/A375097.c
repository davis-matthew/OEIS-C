/*
 * A375097  a(n) = prime(n+2) - (prime(n) + prime(n+1))/2.
 * https://oeis.org/A375097
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+2) - (prime(n)+prime(n+1))/2
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_np1 = (seq_t)get_prime(n + 1);
    seq_t p_np2 = (seq_t)get_prime(n + 2);
    seq_t avg = (p_n + p_np1) / 2;
    return p_np2 - avg;
}
