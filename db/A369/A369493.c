/*
 * A369493  Table read by rows: row n is the unique primitive Pythagorean
 *          triple (a,b,c) such that (a-b+c)/2 = prime(n) and the short leg
 *          "a" is odd.
 * https://oeis.org/A369493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(3k+1) = 2*prime(k) - 1, a(3k+2) = 2*prime(k)^2 - 2*prime(k),
 *            a(3k+3) = 2*prime(k)^2 - 2*prime(k) + 1
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n starts at 1; compute the row index k */
    idx_t k = (n - 1) / 3 + 1;
    /* position within the triple */
    idx_t pos = (n - 1) % 3;

    /* obtain the k-th prime */
    uint32_t p_u32 = get_prime(k);
    seq_t p = (seq_t)p_u32;

    /* compute the three triple components */
    seq_t a_val = (seq_t)2 * p - 1;
    seq_t b_val = (seq_t)2 * p * p - (seq_t)2 * p;   /* 2*p*(p-1) */
    seq_t c_val = b_val + 1;

    /* return the requested term */
    switch (pos) {
        case 0: return a_val;
        case 1: return b_val;
        case 2: return c_val;
        default: return 0; /* unreachable */
    }
}
