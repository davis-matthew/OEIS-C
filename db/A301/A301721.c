/*
 * A301721  Partial sums of A301720.
 * https://oeis.org/A301721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7) (for n >
 *            6) with initial values a(0) = 1, a(1) = 7, a(2) = 16, a(3) = 34,
 *            a(4) = 55, a(5) = 79, a(6) = 115
 * Range:     u8 n <= 9, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *S = NULL;        /* partial sums (A301721) */
    static idx_t  max_computed = 0; /* highest n computed (inclusive) */
    static size_t capacity = 0;     /* current allocated capacity */

    /* ---------- initialise on first call --------------------------------- */
    if (!S) {
        /* allocate enough space for at least n+1 elements, plus a few */
        size_t needed = (size_t)n + 1;
        if (needed < 7) needed = 7;              /* we need first 7 terms */

        /* round up to a power of two for efficient growth */
        capacity = 1;
        while (capacity < needed) capacity <<= 1;

        S = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!S) { /* allocation failure – cannot recover */
            return 0; /* undefined, but we must return a seq_t */
        }

        /* initial 7 terms of A301721 (partial sums of A301720) */
        const seq_t init[7] = { 1, 7, 16, 34, 55, 79, 115 };
        for (idx_t i = 0; i < 7; ++i) {
            S[i] = init[i];
        }

        max_computed = 6;
    }

    /* ---------- extend memoisation if needed ----------------------------- */
    if (n > max_computed) {
        /* make sure capacity is large enough */
        if ((size_t)n + 1 > capacity) {
            size_t new_cap = capacity;
            while ((size_t)n + 1 > new_cap) new_cap <<= 1;
            S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
            if (!S) { /* allocation failure – abort */
                return 0;
            }
            capacity = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence uses only previously computed indices > = 0 */
            seq_t term = S[i - 1]                  /* a(i-1) */
                       + (seq_t)2 * S[i - 3]        /* 2*a(i-3) */
                       - (seq_t)2 * S[i - 4]        /* -2*a(i-4) */
                       - S[i - 6]                  /* -a(i-6) */
                       + S[i - 7];                 /* +a(i-7) */
            S[i] = term;
        }
        max_computed = n;
    }

    return S[n];
}
