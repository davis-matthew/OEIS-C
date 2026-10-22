/*
 * A378137  a(n) = p(n^2*p(n)), where p(x) is the least prime > x.
 * https://oeis.org/A378137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime > n² * p(n), where p(n) is the smallest prime
 *            > n
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 39, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* Find p(n) : the smallest prime greater than n */
    seq_t p = n_seq + 1;
    while (!is_prime_big(p)) {
        p++;
    }

    /* Compute the argument for the outer prime function */
    seq_t target = n_seq * n_seq * p;

    /* Find the least prime greater than target */
    seq_t res = target + 1;
    while (!is_prime_big(res)) {
        res++;
    }

    return res;
}
