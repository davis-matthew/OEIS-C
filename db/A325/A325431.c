/*
 * A325431  a(n) is the least number not 3*a(m) or 4*a(m) for any m < n.
 * https://oeis.org/A325431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least integer not equal to 3*a(m) or 4*a(m) for any m
 *            < n. Equivalently, numbers k whose v₃(k) parity equals the
 *            parity of ⌊v₂(k)/2⌋
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0, guard */

    /* Static cache of already computed terms. */
    static seq_t *seq = NULL;      /* array of terms */
    static size_t len = 0;         /* number of cached terms */
    static size_t cap = 0;         /* allocated capacity of seq */

    /* Next integer to test for inclusion in the sequence. */
    static idx_t next = 1;

    /* Grow the cache until it contains at least n terms. */
    while (len < (size_t)n) {
        idx_t cand = next;
        for (;;) {
            idx_t x = cand;

            /* exponent of 3 dividing cand */
            idx_t e3 = 0;
            while (x % 3 == 0) {
                e3++;
                x /= 3;
            }

            /* exponent of 2 dividing cand */
            idx_t e2 = 0;
            while (x % 2 == 0) {
                e2++;
                x /= 2;
            }

            /* Check parity condition: v3(cand) ≡ ⌊v2(cand)/2⌋ (mod 2) */
            if (((e3 & 1u) == ((e2 >> 1) & 1u))) {
                /* Ensure there is space for the new term. */
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    /* realloc is C; cast to seq_t* for C++ */
                    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    if (!seq) abort();
                    cap = new_cap;
                }
                seq[len++] = (seq_t)cand;   /* store the term */
                next = cand + 1;            /* next candidate */
                break;
            }
            cand++;                       /* try next integer */
        }
    }

    return seq[(size_t)n - 1];
}
