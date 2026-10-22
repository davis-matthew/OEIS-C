/*
 * A309663  Primes that begin a run of consecutive primes whose first
 *          differences are nondecreasing.
 * https://oeis.org/A309663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A309663: primes that begin a run of consecutive primes whose first differences are nondecreasing.
 *  This implementation keeps a running list of run‑starting primes.  The primes are generated on the fly
 *  using the helper function next_prime from "../utils/next_prime.h".  Whenever a run terminates the
 *  next prime starts a new run and is stored.  The first element of the list is 2, corresponding to
 *  the first run 2,3,5,7,11.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/next_prime.h"   /* declares uint64_t next_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* State that persists between calls.  The list of run starts is stored in
     * a dynamic array 'starts'.  'len' is the number of terms currently
     * available.  'cap' is the capacity of that array.  'prev' holds the
     * last prime produced, and 'prev_diff' holds the last gap observed in
     * the current run (0 indicates that the next prime will be the first
     * in a new run).  */
    static seq_t *starts = NULL;   /* array of run‑starting primes */
    static size_t  cap     = 0;     /* capacity of 'starts' */
    static idx_t   len     = 0;     /* number of terms currently stored */

    static uint64_t prev       = 0; /* last prime that was processed */
    static uint64_t prev_diff  = 0; /* difference between the two primes
                                        in the current run (0 => new run) */

    /* ----- initialise on first invocation --------------------------------- */
    if (len == 0) {
        cap   = 8;
        starts = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!starts) abort();            /* fatal error if memory cannot be allocated */

        starts[0] = (seq_t)2;            /* first run starts with prime 2 */
        len   = 1;
        prev  = 2;
        prev_diff = 0;
    }

    /* ----- if the requested term is already known we can return it         */
    if (len >= n) {
        return starts[(size_t)(n - 1)];
    }

    /* ----- otherwise generate primes until we have at least 'n' start terms*/
    while (len < n) {
        uint64_t next = next_prime(prev);   /* next prime after 'prev' */
        uint64_t diff = next - prev;        /* difference between primes */

        if (prev_diff == 0) {
            /* The first prime after a run break.  We record its difference
               for the forthcoming run. */
            prev_diff = diff;
        } else {
            if (diff < prev_diff) {
                /* Gap decreased – the current run has ended.  The prime
                   'next' starts a new run.  Store it.                 */
                if (len == cap) {
                    size_t new_cap = (cap == 0 ? 8 : cap << 1);
                    seq_t *tmp = (seq_t *)realloc(starts, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* fatal error if realloc fails */
                    starts = tmp;
                    cap    = new_cap;
                }
                starts[(size_t)len++] = (seq_t)next;
                prev_diff = 0;                 /* reset for the new run */
            } else {
                /* Gap is nondecreasing – we stay in the same run.    */
                prev_diff = diff;
            }
        }
        prev = next;                        /* advance to next prime for next loop */
    }

    return starts[(size_t)(n - 1)];
}
