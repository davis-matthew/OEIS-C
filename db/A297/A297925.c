/*
 * A297925  Even numbers k such that k - 5 is prime but k - 3 is not prime.
 * https://oeis.org/A297925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p + 5 where p is the nth prime such that p+2 is
 *            composite. The code searches primes incrementally and stores the
 *            sequence in static memory so calls can be made in any order
 * Range:     u8 n <= 35, u16 n <= 5681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Accessor for OEIS A297925 */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* cached sequence values   */
    static size_t seq_len = 0;         /* number of values stored  */
    static size_t seq_cap = 0;         /* allocated capacity       */
    static idx_t next_candidate = 3;   /* next odd number to test  */

    /* Grow the cache until it contains at least n elements. */
    while (seq_len < n) {
        /* Find the next prime number. */
        while (!is_prime(next_candidate))
            next_candidate += 2;       /* skip even numbers        */

        idx_t p = next_candidate;     /* p is prime                */

        /* If p+2 is NOT prime, accept this prime. */
        if (!is_prime(p + 2)) {
            seq_t term = (seq_t)(p + 5); /* even number k            */

            if (seq_len == seq_cap) {   /* enlarge the array if needed */
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = term;
        }

        /* Advance to the next odd candidate for next iteration. */
        next_candidate += 2;
    }

    return seq[n - 1];
}
