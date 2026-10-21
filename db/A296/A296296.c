/*
 * A296296  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n), where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   a_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑1) + a(n‑2) + n*b(n)
   initial values: a(0)=2, a(1)=3;  b(0)=1, b(1)=4, b(2)=5;
   a() and b() are increasing complementary sequences */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a_contains.h"

static void ensure_capacity(seq_t **arr, size_t *cap, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0) ? 2 : (*cap << 1);
        while (new_cap < min_cap) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    static seq_t *A = NULL;      /* a sequence */
    static size_t sizeA = 0, capA = 0;
    static seq_t *B = NULL;      /* b sequence */
    static size_t sizeB = 0, capB = 0;
    static seq_t next_candidate = 0;

    if (sizeA == 0) {
        /* initialise a(0)=2, a(1)=3 */
        ensure_capacity(&A, &capA, 4);
        A[0] = (seq_t)2;
        A[1] = (seq_t)3;
        sizeA = 2;

        /* initialise b(0)=1, b(1)=4, b(2)=5 */
        ensure_capacity(&B, &capB, 4);
        B[0] = (seq_t)1;
        B[1] = (seq_t)4;
        B[2] = (seq_t)5;
        sizeB = 3;

        next_candidate = B[2] + (seq_t)1; /* start after the known b values */
    }

    while (sizeA <= (size_t)n) {
        idx_t i = (idx_t)sizeA;   /* index of the new a term */

        if (sizeB <= (size_t)i) { /* need b(i) */
            while (a_contains(A, sizeA, next_candidate))
                ++next_candidate;     /* find next integer not in a */
            ensure_capacity(&B, &capB, sizeB + 1);
            B[sizeB++] = next_candidate;
            ++next_candidate;          /* advance for the next search */
        }

        seq_t ai = A[sizeA - 1] + A[sizeA - 2] + (seq_t)i * B[i];

        ensure_capacity(&A, &capA, sizeA + 1);
        A[sizeA++] = ai;
    }

    return A[(size_t)n];
}
