/*
 * A327447  a(n) = 4*p(n-1)*p(n+1) - p(n)^2, where p(k) = k-th prime.
 * https://oeis.org/A327447
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*p(n-1)*p(n+1) - p(n)^2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n < 2, but safe return */

    uint32_t p_prev = get_prime(n - 1);
    uint32_t p_cur  = get_prime(n);
    uint32_t p_next = get_prime(n + 1);

    seq_t term = (seq_t)4 * (seq_t)p_prev * (seq_t)p_next;
    seq_t sq   = (seq_t)p_cur * (seq_t)p_cur;

    return term - sq;
}
