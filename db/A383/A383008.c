/*
 * A383008  Indices of the even terms in the sequence of squarefree numbers.
 * https://oeis.org/A383008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A383008
 *  Indices of the even terms in the sequence of squarefree numbers.
 *  Method: enumerate the squarefree numbers in order and count the indices
 *  of those that are even. The first even squarefree number is 2 (index 2),
 *  the next is 6 (index 5), etc.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return the n-th index of an even squarefree number in the list of
 * squarefree numbers (a(1) = 2, a(2) = 5, …).  n ≥ 1.  The function
 * works for any n that fits in the internal counter types and is
 * fast for n up to at least 10 000. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    idx_t even_count = 0;   /* number of even squarefree numbers seen */
    idx_t sq_index  = 0;    /* current index in the squarefree sequence */

    for (idx_t v = 1; ; ++v) {
        /* check whether v is squarefree: for all primes p with p² ≤ v,
         * v % (p²) must not be 0.  We test p from 2 upwards; for the
         * small values required (≤ a few 10⁵) this is fast. */
        bool squarefree = true;
        for (idx_t p = 2; p * p <= v; ++p) {
            idx_t psq = p * p;
            if (v % psq == 0) {
                squarefree = false;
                break;
            }
        }

        if (squarefree) {
            ++sq_index;                     /* advance squarefree index */
            if ((v & 1) == 0) {             /* even squarefree? */
                ++even_count;
                if (even_count == n) {      /* found the desired index */
                    return (seq_t)sq_index;
                }
            }
        }
    }
}
