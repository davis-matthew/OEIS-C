/*
 * A285313  Number of pairs (a,b) such that a*b = n and d(a) = d(b) with d =
 *          A000005 and a <= b.
 * https://oeis.org/A285313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count unordered factor pairs (a,b) of n such that d(a)=d(b)  */
/* d(a) is the number of divisors, computed via divisor_count_u64()          */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t count = 0;
    /* iterate over all divisors a <= sqrt(n) */
    for (idx_t a = 1; a <= n / a; ++a)
    {
        if (n % a == 0)
        {
            idx_t b = n / a;
            seq_t da = divisor_count_u64(a);
            seq_t db = divisor_count_u64(b);
            if (da == db)
                count++;
        }
    }
    return count;
}
