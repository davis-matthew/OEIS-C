/*
 * A377899  a(n) = number of composite numbers c_{2*k+1} <= n, where c_m =
 *          A002808(m) is the m-th composite number.
 * https://oeis.org/A377899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - pi(n))/2) where pi(n) = number of primes ≤ n
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   count_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_primes_upto.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t pi = count_primes_upto(n);
    /* floor((n - pi)/2) – integer division truncates downward */
    return ((seq_t)n - pi) / 2;
}
