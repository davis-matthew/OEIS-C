/*
 * A359369  a(1) = 1. Thereafter a(n) = Sum_{j=1..n} {b(a(j)), where b(a(j)) =
 *          b(a(n))}, and b is A000120.
 * https://oeis.org/A359369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1; for n > 1: w = popcount(a(n-1)) k = number of terms
 *            among a(1)..a(n-1) having weight w a(n) = k * w (Popcount =
 *            number of 1-bits in binary representation)
 * Range:     u8 n <= 230, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Small popcount helper that works for any integer type seq_t. */
static inline unsigned int popcount_seq(seq_t v) {
    unsigned int c = 0;
    while (v) {
        v &= v - 1;  /* delete lowest set one-bit */
        c++;
    }
    return c;
}

/* Compute the sequence value a(n).  The function is memoised and
   works for any call order. */
seq_t a(idx_t n) {
    /* Static state: sequence storage and weight counters */
    static seq_t *A = NULL;          /* index 1..max_n */
    static idx_t  max_n = 0;         /* largest n already computed */
    static uint64_t weight_cnt[128] = {0}; /* count of terms per popcount w */

    /* initialise on first call */
    if (A == NULL) {
        /* small initial capacity */
        size_t cap = 16;
        A = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!A) return (seq_t)0; /* out of memory, return 0 */
        max_n = 1;
        A[1] = (seq_t)1;
        unsigned int w = popcount_seq(A[1]); /* w == 1 */
        weight_cnt[w] = 1;
    }

    /* extend sequence up to required n */
    if (n > max_n) {
        /* Ensure capacity */
        size_t needed = (size_t)n;
        size_t cap = (size_t)max_n;
        while (cap < needed) cap <<= 1;
        if (cap < 16) cap = 16;
        seq_t *newA = (seq_t *)realloc(A, (cap + 1) * sizeof(seq_t));
        if (!newA) return (seq_t)0; /* out of memory */
        A = newA;

        /* compute missing terms */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t prev = A[i - 1];
            unsigned int w_prev = popcount_seq(prev);
            uint64_t k = weight_cnt[w_prev];   /* terms with weight w_prev so far */
            seq_t cur = (seq_t)(k * (seq_t)w_prev);
            A[i] = cur;

            /* update weight counter for the new term */
            unsigned int w_cur = popcount_seq(cur);
            weight_cnt[w_cur] += 1;
        }
        max_n = n;
    }

    return A[n];
}
