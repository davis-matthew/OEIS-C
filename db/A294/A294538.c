/*
 * A294538  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + 2n, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + 2n, a(0) = 1, a(1) = 2, b(0) =
 *            3. The sequence a(n) and its complementary sequence b(n) are
 *            computed iteratively. The arrays A and B are cached statically
 *            so that every call is O(1) after the first up‑to‑n expansion. No
 *            dynamic memory is freed (the program ends i...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- static data ---------------------------------------------------------*/
static seq_t *A = NULL;          /* array of a-values   */
static size_t capA = 0, usedA = 0;   /* capacity & size    */
static seq_t *B = NULL;          /* array of b-values   */
static size_t capB = 0, usedB = 0;   /* capacity & size    */
static idx_t next_candidate = 4;   /* next integer to try for b(n) */
static size_t a_cmp_idx = 0;      /* index in A for membership tests */

/*--- helper: resize an array -------------------------------------------*/
static void ensure_capacity(size_t *Cap, seq_t **Arr, size_t Need)
{
    if (*Cap < Need) {
        size_t NewCap = *Cap ? *Cap * 2 : 16;
        while (NewCap < Need) NewCap <<= 1;
        *Arr = (seq_t *)realloc(*Arr, NewCap * sizeof(seq_t));
        *Cap = NewCap;
    }
}

/*--- main function -------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* one-time initialization ------------------------------------------------*/
    if (!A) {
        /* initialise a(0), a(1) */
        ensure_capacity(&capA, &A, 2);
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        usedA = 2;

        /* initialise b(0) */
        ensure_capacity(&capB, &B, 1);
        B[0] = (seq_t)3;
        usedB = 1;

        /* next candidate is 4 (since 1,2,3 are already used) */
        next_candidate = 4;
        a_cmp_idx = 0;
    }

    /* if the requested index is already known, return it directly */
    if (n < usedA) return A[n];

    /* extend the sequence up to n ------------------------------------------*/
    for (idx_t i = usedA; i <= n; ++i) {
        /* ensure we have enough b-values before computing a[i] */
        while (usedB <= i - 2) {
            /* find the next integer that is not in the existing a-values */
            while (1) {
                seq_t cand = (seq_t)next_candidate;

                /* advance a_cmp_idx until it points to the first a >= cand */
                while (a_cmp_idx < usedA && A[a_cmp_idx] < cand) ++a_cmp_idx;

                if (a_cmp_idx < usedA && A[a_cmp_idx] == cand) {
                    /* cand is already an a-value; skip it */
                    ++next_candidate;
                    continue;
                }

                /* cand is free; store it as the next b-value */
                ensure_capacity(&capB, &B, usedB + 1);
                B[usedB++] = cand;
                ++next_candidate;
                break;
            }
        }

        /* compute a[i] = a[i-1] + a[i-2] + b[i-2] + 2*i */
        seq_t two_i = (seq_t)(2 * (seq_t)i);
        seq_t val = A[i - 1] + A[i - 2] + B[i - 2] + two_i;

        ensure_capacity(&capA, &A, i + 1);
        A[i] = val;
        ++usedA;           /* now indices 0..i are available */
    }

    return A[n];
}
