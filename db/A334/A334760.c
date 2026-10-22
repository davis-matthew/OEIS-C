/*
 * A334760  Perimeters of Pythagorean triangles whose shortest side length
 *          divides its perimeter.
 * https://oeis.org/A334760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334760:
   Perimeters of integer‑sided right triangles whose shortest side
   divides the perimeter.  */
#include <stdlib.h>
#include <stdbool.h>

/* Return the n‑th perimeter (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, but harmless   */
        return 0;

    /* Search bound – will be doubled until enough perimeters are found. */
    seq_t bound = 200;

    /* Repeat until the nth perimeter is found. */
    while (1) {
        /* Mark perimeters up to bound that satisfy the condition. */
        char *mark = (char*)calloc((size_t)(bound + 1), sizeof(char));
        if (!mark)              /* allocation failure – give up          */
            return 0;

        /* Generate all primitive Pythagorean triples with
           perimeter ≤ bound.  */
        for (idx_t m = 2; ; ++m) {
            /* Minimum perimeter for this m (when n_ = 1). */
            seq_t p0_min = 2 * (seq_t)m * ((seq_t)m + 1);
            if (p0_min > bound)
                break;

            for (idx_t n_ = 1; n_ < m; ++n_) {
                /* m and n must be coprime and opposite parity. */
                if (((m - n_) & 1) == 0)
                    continue;
                idx_t a = m, b = n_;
                while (b) {             /* compute gcd(m, n_)                */
                    idx_t t = a % b;
                    a = b;
                    b = t;
                }
                if (a != 1)
                    continue;          /* not coprime                        */

                /* Primitive sides. */
                seq_t m2 = (seq_t)m * (seq_t)m;
                seq_t n2 = (seq_t)n_ * (seq_t)n_;
                seq_t s1 = m2 - n2;                     /* m²–n²       */
                seq_t s2 = 2 * (seq_t)m * (seq_t)n_;    /* 2mn         */
                seq_t s  = s1 < s2 ? s1 : s2;           /* shortest leg */

                /* Primitive perimeter. */
                seq_t P0 = 2 * (seq_t)m * ((seq_t)m + (seq_t)n_);

                /* Condition: shortest leg divides P0. */
                if (P0 % s != 0)
                    continue;          /* this family contributes nothing   */

                /* Mark all multiples of the primitive perimeter. */
                for (seq_t v = P0; v <= bound; v += P0)
                    mark[(size_t)v] = 1;
            }
        }

        /* Count the marked perimeters. */
        seq_t count = 0;
        for (seq_t v = 12; v <= bound; ++v)
            if (mark[(size_t)v])
                ++count;

        if (count >= n) {
            /* Find the nth perimeter. */
            seq_t idx = 0;
            for (seq_t v = 12; v <= bound; ++v) {
                if (mark[(size_t)v]) {
                    ++idx;
                    if (idx == n) {
                        free(mark);
                        return v;
                    }
                }
            }
            free(mark);   /* should never reach here */
            return 0;
        }

        free(mark);
        bound *= 2;          /* enlarge search range and retry */
    }
}
