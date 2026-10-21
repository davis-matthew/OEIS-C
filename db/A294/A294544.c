/*
 * A294544  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + 3, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=a(n-1)+a(n-2)+b(n-1)+3 (n≥2) with
 *            complementary increasing sequences a() and b() and b(0)=3 (b(n)
 *            is the n‑th positive integer not in a())
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Grow a dynamic array of seq_t objects */
static void grow_to(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(**arr));
    *cap = new_cap;
}

/* Compute a(n).  The function is memoized and supports arbitrary
   calling order. */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool init = false;
    static seq_t *A = NULL;        /* array holding a values */
    static seq_t *B = NULL;        /* array holding b values */
    static idx_t a_sz = 0;         /* number of a values stored   */
    static idx_t b_sz = 0;         /* number of b values stored   */
    static size_t a_cap = 0;       /* capacity of A              */
    static size_t b_cap = 0;       /* capacity of B              */
    static seq_t last_b = 0;       /* last b value generated     */
    static idx_t ai_search = 0;    /* index into A for b search  */

    /* Initialise once */
    if (!init) {
        grow_to(&A, &a_cap, 4);
        grow_to(&B, &b_cap, 4);
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        a_sz = 2;
        B[0] = (seq_t)3;
        b_sz = 1;
        last_b = (seq_t)3;
        ai_search = 0;
        init = true;
    }

    /* Extend the tables up to index n */
    while (a_sz <= n) {
        /* Ensure we have b[a_sz-1] ready */
        while (b_sz <= a_sz - (idx_t)1) {
            /* Find the next positive integer not in A[0..a_sz-1] */
            seq_t cand = last_b + (seq_t)1;
            for (;; ) {
                /* Advance ai_search while the a value is less than cand */
                while (ai_search < a_sz && A[ai_search] < cand)
                    ai_search++;
                if (ai_search < a_sz && A[ai_search] == cand) {
                    /* cand is already in A – skip it */
                    cand++;
                    ai_search++;
                    continue;
                }
                /* cand is not in A */
                break;
            }
            grow_to(&B, &b_cap, b_sz + 1);
            B[b_sz] = cand;
            last_b = cand;
            b_sz++;
        }

        /* Compute the next a value */
        seq_t new_a = A[a_sz - 1] + A[a_sz - 2] + B[a_sz - 1] + (seq_t)3;
        grow_to(&A, &a_cap, a_sz + 1);
        A[a_sz] = new_a;
        a_sz++;
    }

    return A[n];
}
