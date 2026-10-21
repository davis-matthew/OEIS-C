/*
 * A263411  "Shuffled bisection" of positive even and odd numbers, type 1 (see
 *          "Comments" for rules generating the sequence).
 * https://oeis.org/A263411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263411: Shuffled bisection of positive even and odd numbers, type 1
 * Algorithm:  a(1) = 1.
 * For each subsequent term:
 *   • If the previous term is even, let o be the next unused odd.
 *     If gcd(o, previous) = 1, use o; otherwise use the next unused even.
 *   • If the previous term is odd, let o be the next unused odd.
 *     If o is composite (i.e., not prime), the odd immediately following the
 *     previous term is composite, so test the next unused even e:
 *         – If gcd(e, previous) = 1, use e.
 *         – Otherwise use o (and consume it).
 *     If o is prime, simply use o and consume it.
 * The process continues indefinitely.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/gcd.h"

static seq_t *cache      = NULL;   /* cached sequence values */
static idx_t cache_len   = 0;      /* number of values cached */
static idx_t cache_cap   = 0;      /* allocated capacity */
static idx_t nextOdd    = 1;       /* next unused odd number */
static idx_t nextEven   = 2;       /* next unused even number */
static seq_t lastTerm   = (seq_t)0;/* last term generated */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Ensure cache has enough space */
    if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp && n > (idx_t)0)
            abort();
        cache      = tmp;
        cache_cap  = n;
    }

    while (cache_len < n) {
        seq_t term;

        /* Generate the first term a(1) = 1 */
        if (cache_len == (idx_t)0) {
            term     = (seq_t)1;
            lastTerm = term;
            nextOdd  = 3;          /* 1 has been consumed */
        }
        /* Previous term is even ------------------------------------------- */
        else if ((lastTerm & (seq_t)1) == (seq_t)0) {
            idx_t candO = nextOdd;                /* next odd */
            if (gcd((seq_t)candO, lastTerm) == (seq_t)1) {
                term = (seq_t)candO;              /* use odd */
                nextOdd += 2;
            } else {
                term = (seq_t)nextEven;           /* use even */
                nextEven += 2;
            }
        }
        /* Previous term is odd -------------------------------------------- */
        else {
            /* Next odd after the previous odd term */
            if (!is_prime_small(nextOdd)) {       /* next odd is composite */
                seq_t evenCand = (seq_t)nextEven; /* try even */
                if (gcd(evenCand, lastTerm) == (seq_t)1) {
                    term = evenCand;             /* use even */
                    nextEven += 2;               /* consume even */
                } else {
                    term = (seq_t)nextOdd;       /* fallback to composite odd */
                    nextOdd += 2;                /* consume that odd */
                }
            } else {                            /* next odd is prime */
                term = (seq_t)nextOdd;           /* use it */
                nextOdd += 2;                    /* consume it */
            }
        }

        cache[cache_len++] = term;
        lastTerm           = term;
    }

    return cache[n - 1];
}
