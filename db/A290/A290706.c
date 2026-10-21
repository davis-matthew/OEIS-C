/*
 * A290706  Greatest of 4 consecutive primes with consecutive gaps 2, 4, 6.
 * https://oeis.org/A290706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A290706: greatest of 4 consecutive primes with gaps 2,4,6.
   a(n) is the largest prime p4 of any block of four consecutive primes
   (p1,p2,p3,p4) satisfying p2‑p1 = 2, p3‑p2 = 4, p4‑p3 = 6.
   The sequence is computed by scanning successive primes using
   the supplied next_prime() routine and keeping a sliding window of
   four consecutive primes.  Each time the window satisfies the
   required gaps, the fourth prime is stored as the next term. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

/* Memoised terms */
static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* allocated capacity */

/* Sliding window of the four most recent primes */
static uint64_t window[4];
static size_t wsize = 0;             /* number of primes currently in window */
static uint64_t last_prime = 2;      /* last prime produced */

/* Append a new term, expanding storage if necessary */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* allocation failure – abort */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but safety */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Fill the initial window of four primes */
        if (wsize < 4) {
            if (wsize == 0) {
                window[0] = last_prime; /* first prime is 2 */
                wsize = 1;
            } else {
                last_prime = next_prime(last_prime + 1);
                window[wsize++] = last_prime;
            }
            continue;
        }

        /* Check the current window for the required gaps */
        if ((window[1] - window[0] == 2) &&
            (window[2] - window[1] == 4) &&
            (window[3] - window[2] == 6)) {
            append_term((seq_t)window[3]); /* store the largest prime */
        }

        /* Shift window left and bring in the next prime */
        window[0] = window[1];
        window[1] = window[2];
        window[2] = window[3];
        last_prime = next_prime(last_prime + 1);
        window[3] = last_prime;
    }

    return terms[n - 1];
}
