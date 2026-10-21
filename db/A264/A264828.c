/*
 * A264828  Nonprimes that are not twice a prime.
 * https://oeis.org/A264828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 9476, u32 n <= 9476, u64 n <= 9476,
 *            u128 n <= 9476, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate integers, keep those that are non‑prime but not of
 * the form 2*p with p prime.  The sequence is produced in order,
 * so a(1)=1, a(2)=8, ... as in OEIS A264828. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t cand = 1;          /* candidate integer to test */
    idx_t count = 0;         /* how many valid terms found so far */

    while (count < n) {
        /* Condition: not prime and not twice a prime */
        if (!is_prime(cand) && !(cand % 2 == 0 && is_prime(cand / 2))) {
            ++count;
            if (count == n)
                return (seq_t)cand;   /* found the n-th term */
        }
        ++cand;
    }
    /* Should never reach here for valid n >= 1 */
    return (seq_t)0;
}
