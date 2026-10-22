/*
 * A398260  a(1)=0; for n>=2, let s = product of the decimal digits of a(n-1);
 *          if s=a(m) for some m < n-1, let m be the greatest such index and
 *          set a(n) = n-1-m; otherwise set a(n) = 0.
 * https://oeis.org/A398260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 455, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>=2 let s = product of decimal digits of a(n-1);
   if s has appeared before, let m be the greatest index < n-1 with a(m)=s;
   then a(n)=n-1-m, otherwise a(n)=0.
   Initial values a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=2, a(6)=0, a(7)=2,
   a(8)=2, a(9)=1, a(10)=6. */
#include <stdlib.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* One‑time initialisation */
    static seq_t *seq = NULL;          /* array of computed a values (index 0->a(1)) */
    static idx_t last = 0;             /* highest n computed so far (1‑based) */
    static size_t cap = 0;             /* allocated capacity of seq */

    /* Ensure we can store up to n terms */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;          /* out‑of‑memory: return 0 as a safe fallback */
        seq[0] = 0;                  /* a(1) = 0 */
        last = 1;
    }
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) return 0;          /* out‑of‑memory */
        cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= last) return seq[n - 1];

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = seq[i - 2];     /* a(i-1) */
        seq_t s = digit_product(prev); /* product of decimal digits of a(i-1) */

        /* Find the most recent earlier occurrence of s */
        idx_t m = 0;                 /* 1‑based index of last occurrence, 0 if none */
        if (i > 2) {
            idx_t j = i - 3;         /* zero‑based index in seq, corresponds to a(i-2) */
            while (true) {
                if (seq[j] == s) {   /* found a match at index j (zero‑based) */
                    m = j + 1;       /* convert to 1‑based index */
                    break;
                }
                if (j == 0) break;   /* reached the beginning */
                --j;
            }
        }

        /* Compute a(i) according to the rule */
        if (m) {
            seq[i - 1] = (seq_t)(i - 1 - m);
        } else {
            seq[i - 1] = 0;
        }
    }

    last = n;
    return seq[n - 1];
}
