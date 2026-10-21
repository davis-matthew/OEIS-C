/*
 * A295960  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) - 1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A295960
    Formula:  a(n) = a(n-1) + a(n-2) + b(n) - 1
    Initial values:  a(0)=1, a(1)=3
                     b(0)=2, b(1)=4, b(2)=5
    b(n) is the least positive integer not belonging to the union
    of {a(0),…,a(n-1)} and {b(0),…,b(n-1)}.
*/

#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *A = NULL;               /* array of a-values          */
static seq_t *B = NULL;               /* array of b-values          */
static size_t cap = 0;                /* current capacity           */
static idx_t max_n = (idx_t)-1;       /* highest index already filled */

/* Ensure array capacity is sufficient for indices up to `required`. */
static void ensure_capacity(idx_t required)
{
    if (required < cap)
        return;
    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap <= (size_t)required)
        new_cap <<= 1;
    seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmpA) exit(1);
    seq_t *tmpB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    if (!tmpB) { free(tmpA); exit(1); }
    A = tmpA;
    B = tmpB;
    cap = new_cap;
}

/* Binary search: does `val` occur in A[0 .. up_to] ? */
static bool used_in_a(seq_t val, idx_t up_to)
{
    idx_t lo = 0, hi = up_to;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (A[mid] == val)
            return true;
        if (A[mid] < val)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return false;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (max_n == (idx_t)-1) {
        ensure_capacity(1);           /* needs indices 0 and 1 */
        A[0] = (seq_t)1;
        B[0] = (seq_t)2;
        A[1] = (seq_t)3;
        B[1] = (seq_t)4;
        max_n = 1;
    }

    if (n <= max_n)
        return A[n];

    ensure_capacity(n);

    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* find next complementary value b(k) */
        seq_t cand = B[k - 1] + (seq_t)1;
        while (used_in_a(cand, k - 1))
            cand += (seq_t)1;
        B[k] = cand;

        /* compute a(k) via the defining recurrence */
        A[k] = A[k - 1] + A[k - 2] + cand - (seq_t)1;
    }

    max_n = n;
    return A[n];
}
