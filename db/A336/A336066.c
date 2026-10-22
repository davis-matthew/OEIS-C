/*
 * A336066  Numbers k such that the exponent of the highest power of 2
 *          dividing k (A007814) is a divisor of k.
 * https://oeis.org/A336066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336066: Numbers k such that the exponent of the highest power of 2
   dividing k (A007814) is a divisor of k.
   The function a(n) returns the n‑th term (1‑based) by scanning
   integers k and testing the condition, caching found terms. */

#include <stddef.h>
#include <stdlib.h>

static inline idx_t v2(seq_t x) {
    idx_t e = 0;
    while ((x & (seq_t)1) == (seq_t)0) {
        x >>= 1;
        e++;
    }
    return e;
}

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t cand = 2;   /* first candidate to test */

    /* Generate terms until we have at least n */
    while (len < n) {
        /* Find the next k satisfying v2(k) | k */
        while (1) {
            idx_t e = v2(cand);
            if (e > 0 && (cand % (seq_t)e) == (seq_t)0)
                break;          /* cand is a valid term */
            cand = cand + (seq_t)1;
        }

        /* Grow cache if needed */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
        seq[len++] = cand;      /* store found term */
        cand = cand + (seq_t)1;  /* next candidate to check */
    }

    return seq[n - 1];
}
