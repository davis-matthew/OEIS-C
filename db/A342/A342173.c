/*
 * A342173  a(n) = Sum_{j=1..n-1} floor(prime(n)/prime(j)).
 * https://oeis.org/A342173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1}^{n-1} floor(prime(n)/prime(j))
 * Range:     u8 n <= 34, u16 n <= 2846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* alias: a(n) = A308495(n) - 2 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t pn = (seq_t)get_prime(n);
    seq_t sum = 0;

    for (idx_t j = 1; j < n; ++j) {
        seq_t pj = (seq_t)get_prime(j);
        sum += pn / pj;
    }

    return sum;
}
