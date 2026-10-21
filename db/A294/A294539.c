/*
 * A294539  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + n - 1, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + b(n‑2) + n – 1 * a(0) = 1, a(1) = 2,
 *            b(0) = 3 * The sequences a() and b() are the complement of one
 *            another. * The function builds them lazily and memoises all
 *            terms so that * calls may be in arbitrary order
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>               /* malloc, realloc */

static seq_t *A = NULL;           /* a[0] … a[lenA-1]   */
static seq_t *B = NULL;           /* b[0] … b[lenB-1]   */
static size_t lenA = 0;           /* number of a terms   */
static size_t lenB = 0;           /* number of b terms   */
static size_t capA = 0;           /* capacity of A       */
static size_t capB = 0;           /* capacity of B       */
static seq_t curUsed = 0;         /* next candidate for b */
static size_t ia = 0, ib = 0;     /* search indices      */
static int initialized = 0;       /* have we seeded A,B? */

/* enlarge an array if necessary */
static void ensure_capacity(size_t *cap, seq_t **ptr, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* find the smallest integer not present in A[0..lenA-1] or B[0..lenB-1] */
static seq_t next_unused(void)
{
    seq_t val = curUsed;
    for (;;) {
        while (ia < lenA && A[ia] < val) ia++;
        if (ia < lenA && A[ia] == val) { val++; continue; }

        while (ib < lenB && B[ib] < val) ib++;
        if (ib < lenB && B[ib] == val) { val++; continue; }

        break;
    }
    curUsed = val + 1;
    return val;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    /*  base cases  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /*  initialise static state once  */
    if (!initialized) {
        ensure_capacity(&capA, &A, 2);
        ensure_capacity(&capB, &B, 1);

        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        lenA = 2;

        B[0] = (seq_t)3;
        lenB = 1;

        curUsed = (seq_t)4;     /* smallest unused after 1,2,3   */
        ia = 0;
        ib = 0;
        initialized = 1;
    }

    /*  grow arrays until a[n] has been computed  */
    while (lenA <= (size_t)n) {
        size_t k = lenA;                           /* next index to compute */

        /* a[k] = a(k-1)+a(k-2)+b(k-2)+(k-1) */
        seq_t a_k = A[k-1] + A[k-2] + B[k-2] + (seq_t)(k-1);

        ensure_capacity(&capA, &A, lenA + 1);
        A[lenA] = a_k;
        lenA++;

        /* b[k-1] = smallest unused integer  */
        ensure_capacity(&capB, &B, lenB + 1);
        B[lenB] = next_unused();
        lenB++;
    }

    return A[(size_t)n];
}
