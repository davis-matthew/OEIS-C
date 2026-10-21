/*
 * A267215  Integers k such that k! is the sum of 3 squares.
 * https://oeis.org/A267215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k (starting from k = 0) such that * k!
 *            is a sum of three squares. Equivalently, k! is not of the *
 *            forbidden form 4^a*(8b+7). A value k! is representable as a sum
 *            of * three squares iff the exponent of 2 in k! is odd, or the
 *            odd part of * k! is not congruent to 7 modulo 8
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;

    /* cache the fact that 0! = 1 is representable (odd part = 1) */
    if (sz == 0) {
        cap = 256;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;          /* k = 0 */
        sz = 1;
    }

    /* v2 : exponent of 2 in current k!  (starting at k = 0!)
     * oddprod : product modulo 8 of odd parts of integers up to k   */
    static unsigned v2 = 0;
    static unsigned oddprod = 1;      /* 1 when k = 0!                 */

    /* current k processed (starts at 0) */
    static idx_t cur_k = 0;

    /* generate terms until we have n of them */
    while (sz < n) {
        /* move to next k */
        cur_k++;

        /* add the new factor cur_k to the running factorial */
        idx_t tmp = cur_k;
        while ((tmp & 1) == 0) {      /* count powers of 2 in cur_k   */
            tmp >>= 1;
            v2++;
        }
        /* tmp now odd: multiply its value modulo 8 into oddprod */
        oddprod = (oddprod * (tmp & 7u)) & 7u;

        /* check representability:
         * forbidden if v2 even and oddprod == 7
         */
        if (!((v2 & 1u) == 0 && oddprod == 7u)) {
            /* store term */
            if (sz == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = (seq_t)cur_k;
        }
    }

    return cache[n - 1];
}
