/*
 * A293227  a(n) is the number of proper divisors of n that are squarefree.
 * https://oeis.org/A293227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) - 1_{squarefree}(n)
 * Range:     u8 n <= 7473, u16 n <= 9208, u32 n <= 9208, u64 n <= 9208,
 *            u128 n <= 9208, big any n
 * Helpers:   count_distinct_primes, is_squarefree, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the distinct prime factor count and
 * square‑free test. */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset is 1). */
    int omega = count_distinct_primes((uint64_t)n);          /* number of distinct primes in n */
    seq_t result = pow2((idx_t)omega);                     /* 2^omega(n) */
    if (is_squarefree(n))                                 /* mu(n)^2 = 1 if n squarefree */
        result -= 1;                                      /* subtract 1, else subtract 0 */
    return result;
}
