/*
 * A374353  Least k such that prime(n) mod k = n or 0 if no such k exists.
 * https://oeis.org/A374353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that prime(n) mod k = n (0 if no such k
 *            exists). Note that prime(n) is the n‑th prime, with 1‑based
 *            indexing
 * Range:     u8 n <= 66, u16 n <= 7193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   find_min_div_greater, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/find_min_div_greater.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1.  For n <= 4 the answer is 0. */
    if (n <= 4) return 0;

    /* Get the n-th prime.  get_prime(1)=2, etc.  The function returns
       a uint32_t, which is safe up to very large n for our use case. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Compute difference prime(n) – n. */
    seq_t diff = p - (seq_t)n;

    /* Find least divisor > n.  If none, return 0. */
    return find_min_div_greater(diff, (seq_t)n);
}
