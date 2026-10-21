/*
 * A271512  a(n) = (p+1)*(p+2)*(p+3)/6 where p is the n-th prime.
 * https://oeis.org/A271512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p(n)+1)*(p(n)+2)*(p(n)+3)/6 where p(n) is the n-th prime
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 325, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the get_prime utility from ../utils/get_prime.h */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* safety: OEIS offset starts at 1 */
    uint32_t p = get_prime(n);       /* 1‑based index */
    seq_t pp = (seq_t)p;
    seq_t val = (pp + 1) * (pp + 2) * (pp + 3) / 6;
    return val;
}
