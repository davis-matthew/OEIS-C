/*
 * A291745  Nonprimes of the form 3*k + 1.
 * https://oeis.org/A291745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291745: Nonprimes of the form 3*k + 1.
   a(n) is the n-th number of the form 3k+1 that is not prime
   (a(1) = 1, a(2) = 4, a(3) = 10, …).  The function
   iteratively tests successive numbers 3k+1 for primality
   and counts the nonprimes until the n‑th one is found. */

#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* out of range; not used by the test harness */
        return 0;

    seq_t cand = 1;      /* first number of the form 3k+1 */
    idx_t  count = 0;    /* how many nonprimes found so far */

    while (1) {
        if (!is_prime_small(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
        cand += 3;       /* next 3k+1 */
    }
}
