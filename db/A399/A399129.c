/*
 * A399129  Primes of the forms 10*k+1 and 10*k+9 for which the number of
 *          primes of the forms 10*k+1 and 10*k+9 are equal.
 * https://oeis.org/A399129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence: A399129 – primes of the forms 10*k+1 and 10*k+9 for which the
 * number of primes of each form seen so far are equal.
 * The function computes the n‑th term by scanning primes and maintaining
 * two counters for the two residue classes.  Whenever the counters are
 * equal the prime is appended to the result list.
 * The implementation is fully self‑contained, uses only the supplied
 * prime test routine, and tolerates arbitrary‑precision seq_t.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t  cap  = 0;
    static size_t  len  = 0;
    static idx_t   cur  = 10;          /* last tested number; starts at 10 */
    static idx_t   cnt1 = 0;           /* #primes ≡ 1 (mod 10) seen so far */
    static idx_t   cnt9 = 0;           /* #primes ≡ 9 (mod 10) seen so far */

    while (len < n) {
        /* search for the next prime of form 10*k+1 or 10*k+9 */
        do {
            ++cur;
        } while ( (cur % 10 != 1 && cur % 10 != 9) || !is_prime(cur) );

        /* update the appropriate counter */
        if (cur % 10 == 1)
            ++cnt1;
        else
            ++cnt9;

        /* record the prime when the counts are equal */
        if (cnt1 == cnt9) {
            if (len == cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    free(terms);
                    abort();          /* allocation failure */
                }
                terms = tmp;
                cap   = new_cap;
            }
            terms[len++] = (seq_t)cur;
        }
    }
    return terms[n - 1];
}
