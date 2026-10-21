/*
 * A267896  a(n) = (Prime(n+1)^2 - Prime(n)^2) / 8.
 * https://oeis.org/A267896
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Prime(n+1)^2 - Prime(n)^2) / 8
 * Range:     u8 n <= 8, u16 n <= 1057, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is the OEIS index; a(2) is the first listed term. */
    uint32_t p_n   = get_prime(n);     /* Prime(n)   */
    uint32_t p_np1 = get_prime(n + 1); /* Prime(n+1) */

    seq_t sq_n   = (seq_t)p_n   * (seq_t)p_n;
    seq_t sq_np1 = (seq_t)p_np1 * (seq_t)p_np1;

    seq_t diff = sq_np1 - sq_n;
    return diff / (seq_t)8;
}
