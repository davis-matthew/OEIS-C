/*
 * A294423  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) - b(n-2) + n, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   expand_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = a(n-1) + a(n-2) + b(n-1) - b(n-2) + n
 *  a(0) = 1, a(1) = 3
 *  b(0) = 2, b(1) = 4
 *
 *  The complementary sequence b() is the set of positive integers
 *  not appearing in a().  We build a[] and b[] lazily, caching all
 *  previously computed terms so that every call to a(idx_t n) works
 *  independently of call order.  A boolean bitmap keeps track of
 *  which small integers have already been used, allowing us to find
 *  the next b-value in O(1) amortised time.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/expand_array.h"

/* static state shared between calls */
static seq_t *A = NULL;          /* a-values */
static idx_t  szA = 0;           /* number of a-values stored */
static size_t capA = 0;          /* allocated capacity for A */

static seq_t *B = NULL;          /* b-values */
static idx_t  szB = 0;           /* number of b-values stored */
static size_t capB = 0;          /* allocated capacity for B */

static bool *used = NULL;        /* boolean bitmap for used integers */
static size_t capUsed = 0;       /* allocated bitmap size */
static idx_t nextCandidate = 1;  /* next integer to try for b() */

/* ensure bitmap can index up to 'limit' (inclusive) */
static void expand_used(size_t limit)
{
    if (limit < capUsed) return;
    size_t newCap = (capUsed == 0) ? 64 : capUsed;
    while (newCap <= limit) newCap <<= 1;
    bool *tmp = (bool *)realloc(used, newCap * sizeof(bool));
    if (tmp == NULL) {
        /* allocation failed – abort; caller will not use this build */
        exit(EXIT_FAILURE);
    }
    /* initialise new entries to false */
    for (size_t i = capUsed; i < newCap; ++i) tmp[i] = false;
    used = tmp;
    capUsed = newCap;
}

/* initialise state on first call */
static void init_state(void)
{
    if (A != NULL) return;  /* already initialised */

    /* a(0)=1, b(0)=2 */
    expand_array(&A, &capA, &szA);
    expand_array(&B, &capB, &szB);
    expand_used(4);  /* to hold values 1..4 initially */

    A[szA++] = (seq_t)1;  /* a(0) */
    B[szB++] = (seq_t)2;  /* b(0) */
    used[1] = true;
    used[2] = true;

    A[szA++] = (seq_t)3;  /* a(1) */
    B[szB++] = (seq_t)4;  /* b(1) */
    used[3] = true;
    used[4] = true;

    /* next unused integer */
    nextCandidate = 5;
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    init_state();

    /* compute all terms up to n if necessary */
    while (szA <= n) {
        idx_t i = szA;  /* current index */

        /* compute a(i) using the recurrence */
        seq_t diff   = B[i-1] - B[i-2];
        seq_t term   = A[i-1] + A[i-2] + diff + (seq_t)i;
        expand_array(&A, &capA, &szA);
        A[szA++] = term;

        /* mark a(i) in bitmap if it fits */
        if ((idx_t)term < (idx_t)capUsed) {
            used[(size_t)term] = true;
        } else if ((idx_t)term < (idx_t)nextCandidate) {
            /* if a(i) is small but > used size, enlarge bitmap */
            expand_used((size_t)term);
            used[(size_t)term] = true;
        }

        /* find next unused integer -> b(i) */
        while (true) {
            if (nextCandidate >= (idx_t)capUsed) {
                /* enlarge bitmap sufficiently */
                expand_used((size_t)nextCandidate);
            }
            if (!used[(size_t)nextCandidate]) break;
            nextCandidate++;
        }
        seq_t bval = (seq_t)nextCandidate;
        expand_array(&B, &capB, &szB);
        B[szB++] = bval;
        used[(size_t)nextCandidate] = true;
        nextCandidate++;
    }

    return A[(size_t)n];
}
