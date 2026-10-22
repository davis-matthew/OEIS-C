/*
 * A309701  Primes with record Manhattan distance from origin. When starting
 *          rightwards in a grid, turn left after a prime number. If not, walk
 *          straight on.
 * https://oeis.org/A309701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 148, u32 n <= 166, u64 n <= 166,
 *            u128 n <= 166, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309701 - primes that set a new record Manhattan distance in a
   walk that starts rightward and turns left after a prime-numbered step.
   The function simulates the walk, keeps track of the maximum distance,
   and records a step index if it is prime and produces a new maximum. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* State persisted between calls */
    static seq_t *record_primes = NULL;    /* array of recorded primes */
    static size_t rec_cap = 0;             /* current capacity */
    static size_t rec_cnt = 0;             /* number of recorded primes */

    static idx_t step = 0;                 /* current step number (1‑based) */
    static int64_t x = 0, y = 0;           /* current position */
    static int dir = 0;                    /* 0=E,1=N,2=W,3=S */
    static seq_t maxdist = 0;              /* maximum Manhattan distance seen */

    /* Expand the array of recorded primes as needed */
    while (rec_cnt < n) {
        /* Advance one step */
        step++;

        switch (dir) {
            case 0: x++; break; /* east   */
            case 1: y++; break; /* north  */
            case 2: x--; break; /* west   */
            case 3: y--; break; /* south  */
        }

        /* Manhattan distance from origin */
        seq_t dist = (seq_t)((x >= 0 ? (uint64_t)x : (uint64_t)(-x)) +
                             (y >= 0 ? (uint64_t)y : (uint64_t)(-y)));

        /* New record distance? */
        if (dist > maxdist) {
            maxdist = dist;
            /* Record the step number if it is prime */
            if (is_prime(step)) {
                grow_seq(&record_primes, &rec_cap, rec_cnt + 1);
                record_primes[rec_cnt++] = (seq_t)step;
            }
        }

        /* Turn left after a prime‑numbered step */
        if (is_prime(step))
            dir = (dir + 1) & 3;
    }

    return record_primes[(size_t)n - 1];
}
