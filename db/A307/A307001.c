/*
 * A307001  Odd numbers > 1 not of the form (3n*k - n - k + 1)/2 where n and k
 *          are odd numbers > 1.
 * https://oeis.org/A307001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd numbers >1 not of the form (3*n*k - n - k + 1)/2
   where n,k are odd numbers >1.  The function searches for the
   n‑th such odd number, generating all representable values up to
   a progressively increasing bound until the desired count is
   reached.  This is a moderate sieve algorithm that works for
   arbitrary precision seq_t as well as for the 64‑bit case. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*  a(idx_t n) returns the n‑th term of A307001 (n ≥ 1).      */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    idx_t limit = 8 * n;                 /* initial bound guess */

    for (;;) {
        /* Allocate a zeroed array of bytes; cast to avoid C++ warning. */
        uint8_t *rep = (uint8_t *) calloc((size_t)limit + 1, sizeof(uint8_t));
        if (!rep) return (seq_t)0;        /* out of memory fallback */

        /* Mark all numbers <= limit that are of the form 6ab+2a+2b+1,
           with a,b >= 1. */
        for (idx_t a = 1; 8 * a + 3 <= limit; ++a) {
            idx_t val = 8 * a + 3;         /* first value for this a   */
            idx_t inc = 6 * a + 2;         /* step when b increases   */
            while (val <= limit) {
                rep[val] = 1;
                val += inc;
            }
        }

        /* Scan odd numbers >1, counting those not marked. */
        idx_t count = 0;
        seq_t answer = 0;
        for (idx_t v = 3; v <= limit; v += 2) {
            if (!rep[v]) {                 /* not representable */
                ++count;
                if (count == n) {
                    answer = (seq_t)v;
                    break;
                }
            }
        }

        free((void *)rep);

        if (count >= n) {
            return answer;                 /* found the term */
        }

        /* Not enough terms found yet; double the search bound. */
        limit <<= 1;
    }
}
