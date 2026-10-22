/*
 * A359376  Numbers that are either odd multiples of 3 or multiples of 4.
 *          Numbers k such that A252463(k) is even.
 * https://oeis.org/A359376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k ≥ 0 such that k mod 12 ∈ {0,3,4,8,9}
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;    /* 1‑based array of the generated terms */
    static idx_t size = 0;       /* number of stored terms */
    static idx_t cap  = 0;       /* allocated capacity (≥ size + 1) */
    static seq_t next_k = 0;     /* next candidate integer to test */

    /* Allocate initial storage on the first call */
    if (!seq) {
        cap = 16;
        seq = (seq_t *)malloc((size_t)(cap + 1) * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failed – give up */
    }

    /* Generate terms until we have at least n terms */
    while (size < n) {
        seq_t r = next_k % 12;
        if (r == 0 || r == 3 || r == 4 || r == 8 || r == 9) {
            /* grow storage if necessary */
            if (size + 1 > cap) {
                cap = cap * 2;
                seq = (seq_t *)realloc(seq, (size_t)(cap + 1) * sizeof(seq_t));
                if (!seq) return 0;  /* allocation failed – give up */
            }
            seq[++size] = next_k;   /* store the new term */
        }
        ++next_k;
    }

    return seq[n];
}
