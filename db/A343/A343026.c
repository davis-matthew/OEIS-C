/*
 * A343026  a(n) is the smallest positive integer such that there exists no
 *          positive integer k with 2k < n where a(n-2k)*a(n-k)=a(n).
 * https://oeis.org/A343026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest positive integer x such that for all k>0 with 2k < n,
 *        a(n-2k) * a(n-k) != x  */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *A = NULL;   /* 1‑based array of computed terms */
    static idx_t size = 0;    /* current allocated size (max index computed) */

    if (n == 0) return 0;     /* not used for the OEIS offset 1 */

    if (n > size) {
        idx_t new_size = n;
        A = (seq_t *)realloc(A, (new_size + 1) * sizeof(seq_t));
        if (!A) exit(1);      /* memory allocation failed */

        /* Compute missing terms from (size+1) up to n */
        for (idx_t i = size + 1; i <= new_size; ++i) {
            seq_t val = 1;            /* candidate value */
            for (;;) {                /* find smallest valid value */
                bool ok = true;
                for (idx_t k = 1; 2 * k < i; ++k) {
                    if (A[i - 2 * k] * A[i - k] == val) {
                        ok = false;   /* candidate invalid */
                        break;
                    }
                }
                if (ok) break;
                ++val;                /* try next integer */
            }
            A[i] = val;               /* store the computed term */
        }
        size = new_size;             /* update current size */
    }

    return A[n];
}
