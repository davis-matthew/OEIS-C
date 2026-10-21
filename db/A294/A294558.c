/*
 * A294558  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + 2*n, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + 2*n with a(0)=1,
 *            a(1)=2; b(0)=3; a() and b() are increasing complementary
 *            sequences
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- static storage for memoisation ---------- */
static seq_t *A = NULL;          /* a[0] … a[lenA-1] */
static seq_t *B = NULL;          /* b[0] … b[lenB-1] */
static size_t lenA = 0, lenB = 0;
static size_t capA = 0, capB = 0;
static seq_t candidate = 0;      /* next possible value for b */
static idx_t idxA = 0;           /* first a[idxA] >= current candidate */
static bool init_done = false;

/* ensure capacity for array of seq_t */
static void ensure_cap(size_t *cap, seq_t **arr, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* initialise base values */
static void init(void)
{
    if (init_done) return;

    /* initialise a[0] = 1, a[1] = 2 */
    ensure_cap(&capA, &A, 4);
    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    lenA = 2;

    /* initialise b[0] = 3 */
    ensure_cap(&capB, &B, 4);
    B[0] = (seq_t)3;
    lenB = 1;

    /* first unused candidate after b[0] */
    candidate = (seq_t)4;

    /* idxA points to first a[idxA] >= candidate */
    while (idxA < lenA && A[idxA] < candidate) idxA++;

    init_done = true;
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    init();

    if (n < lenA) return A[n];

    /* compute a[] up to n */
    for (idx_t i = (idx_t)lenA; i <= n; ++i) {
        /* generate b[i-1] if not yet present */
        while (lenB <= i-1) {
            /* find next unused integer for b */
            while (true) {
                while (idxA < lenA && A[idxA] < candidate) idxA++;
                if (idxA < lenA && A[idxA] == candidate) {
                    candidate++;   /* a cannot be used, skip to next */
                    idxA++;        /* skip this a as well */
                } else {
                    break;        /* candidate is unused */
                }
            }
            /* store new b */
            ensure_cap(&capB, &B, lenB + 1);
            B[lenB++] = candidate;
            candidate++;          /* next possible candidate */
        }

        /* compute a[i] using recurrence */
        seq_t ai = A[i-1] + A[i-2] + B[i-1] + B[i-2] + ( (seq_t)i * 2 );

        /* store a[i] */
        ensure_cap(&capA, &A, lenA + 1);
        A[lenA++] = ai;
    }

    return A[n];
}
