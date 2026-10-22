/*
 * A395068  a(n) = Sum_{1 <= i < j <= n} ( n!/(i*j) )^4.
 * https://oeis.org/A395068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + (n-1)^4)*a(n-1) - (n-1)^8*a(n-2) + ((n-2)!)^4 for
 *            n > 1 a(0) = 0, a(1) = 0 Implementation uses an incremental
 *            recurrence and memoisation. The routine works for any seq_t
 *            (built‑in integer or arbitrary‑precision integer) and any idx_t
 *            (uint64_t)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>  /* size_t */

/* Ensure a dynamically allocated array has at least "needed" elements.
   The array is resized to the next power of two >= needed. */
static void ensure_capacity(size_t *cap, size_t needed, seq_t **arr) {
    if (*cap < needed) {
        size_t newcap = (*cap > 1) ? *cap : 4;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *arr = tmp;
        *cap = newcap;
    }
}

/* Computes the n-th term of sequence A395068. */
seq_t a(idx_t n) {
    /* Static caches for the sequence values and for factorial accumulation. */
    static seq_t *A = NULL;       /* sequence values */
    static size_t capA = 0, lenA = 0;   /* allocated capacity and number of stored terms */

    static seq_t f_fact = 1;     /* factorial of (lenA-1) */
    static seq_t f_fact4 = 1;    /* (factorial of (lenA-1))^4 */

    /* Initialise cache for the first two terms (n = 0, 1). */
    if (lenA < 2) {
        ensure_capacity(&capA, 2, &A);
        A[0] = 0;
        A[1] = 0;
        lenA = 2;
        /* f_fact and f_fact4 are already 1 (0! = 1, 0!^4 = 1). */
    }

    if ((size_t)n >= lenA) {
        /* Ensure sufficient allocation. */
        ensure_capacity(&capA, (size_t)n + 1, &A);

        /* Compute terms from lenA up to n inclusive. */
        for (idx_t i = (idx_t)lenA; i <= n; ++i) {
            /* Powers needed in the recurrence. */
            seq_t i_seq = (seq_t)i;
            seq_t ip1 = (seq_t)(i - 1);

            seq_t i2   = i_seq * i_seq;                 /* i^2  */
            seq_t i4   = i2 * i2;                       /* i^4  */

            seq_t ip1_2 = ip1 * ip1;
            seq_t ip1_4 = ip1_2 * ip1_2;                /* (i-1)^4 */
            seq_t coeff1 = i4 + ip1_4;                  /* i^4 + (i-1)^4 */

            seq_t coeff2 = ip1_4 * ip1_4;                /* (i-1)^8 */

            seq_t term1 = coeff1 * A[i - 1];
            seq_t term2 = coeff2 * A[i - 2];

            /* f_fact4 currently holds ( (i-2)! )^4. */
            seq_t val = term1 - term2 + f_fact4;
            A[i] = val;

            /* Update factorial accumulators for the next iteration. */
            f_fact = f_fact * ip1;          /* (i-1)! from (i-2)! */
            f_fact4 = f_fact4 * ip1_4;      /* ((i-1)!)^4 from ((i-2)!)^4 */
        }
        lenA = (size_t)n + 1;
    }

    return A[(size_t)n];
}
