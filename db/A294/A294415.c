/*
 * A294415  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + 1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula : a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) + 1,
   a(0)=1, a(1)=3, b(0)=2, b(1)=4
   The arrays A[] and B[] store the sequences; they are filled on demand
   up to the largest index requested so far.  `next` holds the smallest
   positive integer that has not yet appeared in either array.  Because
   A[] and B[] are strictly increasing, we can skip used values by
   advancing two indices. */
  
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static state persistent across calls */
    static seq_t *A = NULL;
    static seq_t *B = NULL;
    static idx_t  cap = 0;   /* allocated capacity */
    static idx_t  size = 0;  /* number of a-values stored */
    static idx_t  bsize = 0; /* number of b-values stored */
    static idx_t  next = 1;  /* smallest candidate for the next b-value */
    static idx_t  i_a = 0, i_b = 0; /* indices for scanning used values */

    /* one‑time allocation and initialisation */
    if (cap == 0) {
        cap = 16;
        A   = (seq_t*)malloc(cap * sizeof(seq_t));
        B   = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!A || !B) return (seq_t)0;   /* allocation failure – safe return */

        /* seed the base cases */
        A[0] = (seq_t)1;   A[1] = (seq_t)3;
        B[0] = (seq_t)2;   B[1] = (seq_t)4;
        size  = 2;
        bsize = 2;

        /* initialise `next` to the first integer not in {1,2,3,4} */
        next = 1;
        i_a = 0;
        i_b = 0;
        while ((i_a < size && A[i_a] == (seq_t)next) ||
               (i_b < bsize && B[i_b] == (seq_t)next)) {
            if (i_a < size && A[i_a] == (seq_t)next) i_a++;
            if (i_b < bsize && B[i_b] == (seq_t)next) i_b++;
            next++;
        }
    }

    /* extend the tables up to index `n` if necessary */
    if (n >= size) {
        /* make sure we have enough capacity */
        idx_t need = n + 1;
        if (need > cap) {
            while (cap < need) cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(A, cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            A = tmp;
            tmp = (seq_t*)realloc(B, cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            B = tmp;
        }

        /* compute the missing terms */
        for (idx_t k = size; k <= n; ++k) {
            /* recurrence for a(k) – indices k-1 and k-2 are already known */
            A[k] = A[k-1] + A[k-2] + B[k-1] + B[k-2] + (seq_t)1;

            /* find the next unused integer for b(k) */
            while ((i_a < A[k] + 2 && A[i_a] == (seq_t)next) ||
                   (i_b < B[k] + 1 && B[i_b] == (seq_t)next)) {
                if (i_a < A[k] + 2 && A[i_a] == (seq_t)next) i_a++;
                if (i_b < B[k] + 1 && B[i_b] == (seq_t)next) i_b++;
                next++;
            }

            B[k] = (seq_t)next;
            next++;          /* next candidate for future positions */

            /* advance the scanning indices past the newly inserted values */
            while (i_a < k + 1 && A[i_a] == B[k]) i_a++;   /* b(k) might equal a[some] but this case never occurs */
            if (i_b == k) i_b = k;  /* new b-value just stored at index k */
        }

        size  = n + 1;
        bsize = n + 1;
    }

    return A[n];
}
