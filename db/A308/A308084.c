/*
 * A308084  a(n) = n*(n-1)*d(n)/4, where d(n)=A000005(n) is the number of
 *          divisors of n.
 * https://oeis.org/A308084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*d(n)/4, where d(n) is the number of divisors of n
 * Range:     u8 n <= 9, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    /* Convert index to the sequence type before computations to avoid
       promotion to int and preserve exactness in the target type. */
    seq_t nn   = (seq_t)n;
    seq_t n_m1 = nn - 1;
    seq_t d    = (seq_t)divisor_count_u64((uint64_t)n);

    /* Compute n*(n-1)*d then divide by 4.  All operations are performed in
       seq_t; intermediate overflows wrap around as per the type's size,
       which is the intended behaviour for values that do not fit. */
    seq_t res = nn * n_m1;
    res *= d;
    res /= 4;
    return res;
}
