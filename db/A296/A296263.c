/*
 * A296263  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n-1), where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n-1), a(0) = 2, a(1) = 3; b(0)
 *            = 1, b(1) = 4; (a(n)) and (b(n)) are increasing complementary
 *            sequences
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static storage for computed values */
    static seq_t *A = NULL;   /* a sequence */
    static seq_t *B = NULL;   /* b sequence (complement) */
    static size_t len = 0;    /* number of elements stored so far (indices 0..len-1) */
    static size_t cap = 0;    /* array capacity */
    static seq_t next_b = 0;  /* next candidate for the b sequence */
    static size_t a_check = 0;/* index used when skipping a values */

    /* initialisation – executed once */
    if (len == 0) {
        cap = 16;
        A = (seq_t*)malloc(cap * sizeof(seq_t));
        B = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!A || !B) return 0; /* allocation failed */

        /* initial terms */
        A[0] = 2; B[0] = 1;
        A[1] = 3; B[1] = 4;

        len = 2;                 /* we have indices 0 and 1 */
        next_b = 5;              /* next candidate for b[2] */
        a_check = 0;             /* start scan from A[0] */
    }

    if (n < len)
        return A[(size_t)n];

    size_t target = (size_t)n;

    /* grow arrays if needed */
    if (cap < target + 1) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap < target + 1)
            new_cap <<= 1;
        seq_t *newA = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        seq_t *newB = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
        if (!newA || !newB) return 0;
        A = newA;
        B = newB;
        cap = new_cap;
    }

    /* build terms until we reach index target */
    for (size_t i = len; i <= target; ++i) {
        /* compute a[i] (i >= 2) */
        seq_t new_a = A[i-1] + A[i-2] + B[i-2] * B[i-1];
        A[i] = new_a;

        /* find next b[i] – the smallest integer > previous b
           that is not in a[0..i] */
        seq_t cand = next_b;
        while (1) {
            /* advance a_check until it points to a value >= cand */
            while (a_check < i+1 && A[a_check] < cand)
                ++a_check;
            if (a_check < i+1 && A[a_check] == cand) {
                ++cand; /* skip a value */
                continue;
            }
            break;      /* cand is free */
        }
        B[i] = cand;
        next_b = cand + 1;
    }
    len = target + 1;

    return A[target];
}
