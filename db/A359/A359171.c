/*
 * A359171  Nonmultiples of 3 that have an even number of prime factors (with
 *          multiplicity).
 * https://oeis.org/A359171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that k % 3 != 0 and Ω(k)
 *            (prime factors with multiplicity) is even
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359171: Nonmultiples of 3 that have an even number of prime factors (with multiplicity). */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    idx_t count = 0;         /* how many terms we have found so far */
    idx_t x = 0;             /* candidate integer to test */

    while (count < n)
    {
        ++x;                 /* examine next integer */

        if (x % 3ull != 0)          /* not divisible by 3? */
        {
            /* count prime factors (with multiplicity) of x */
            seq_t omega = count_prime_factors((seq_t)x);

            /* check parity of omega */
            if ((omega % (seq_t)2) == 0)
            {
                ++count;         /* found a valid term */
                if (count == n)  /* reached requested index */
                    return (seq_t)x;
            }
        }
    }
    return 0; /* should never reach here for n >= 1 */
}
