/*
 * A357901  a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n - 2*k,k)|.
 * https://oeis.org/A357901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n - 2*k,k)|
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function a should have external linkage; seq_t and idx_t are
   provided by the build system. The implementation below uses
   dynamic allocation and memoisation so that repeated calls in any
   order are efficient. */
seq_t a(idx_t n)
{
    /* Static table of unsigned Stirling numbers of the first kind.
       The table is stored as a single contiguous array, where the
       entry St(n,k) is placed at index offset(n)+k where
       offset(n) = n*(n+1)/2.  The table contains all values for
       0 <= n <= current_max_n. */
    static seq_t *tri = NULL;          /* pointer to the table */
    static idx_t current_max_n = 0;    /* highest n for which the table is built */
    static size_t table_size = 0;      /* allocated number of seq_t entries */

    /* Initialise on first call */
    if (tri == NULL) {
        /* Need space for a single entry (n=0,k=0) */
        table_size = 1;
        tri = (seq_t*)malloc(table_size * sizeof(seq_t));
        if (tri == NULL) return 0;   /* out of memory: return 0 */
        tri[0] = (seq_t)1;           /* |s(0,0)| = 1 */
        current_max_n = 0;
    }

    /* Grow the table if necessary */
    if (n > current_max_n) {
        /* New required size: (n+1)*(n+2)/2 entries */
        size_t new_table_size = (size_t)(n + 1) * (n + 2) / 2;
        if (new_table_size > table_size) {
            seq_t *tmp = (seq_t*)realloc(tri, new_table_size * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* out of memory: return 0 */
            tri = tmp;
            table_size = new_table_size;
        }

        /* Compute Stirling numbers row by row up to n */
        for (idx_t m = current_max_n + 1; m <= n; ++m) {
            size_t offset_m = (size_t)m * (m + 1) / 2;            /* offset for row m */
            size_t offset_prev = (size_t)(m - 1) * m / 2;          /* offset for row m-1 */

            for (idx_t k = 0; k <= m; ++k) {
                size_t idx = offset_m + k;
                if (m == 0 && k == 0) {
                    tri[idx] = (seq_t)1;
                } else if (k == 0) {
                    tri[idx] = (seq_t)0;
                } else {
                    seq_t prev_k_minus_1 = tri[offset_prev + (k - 1)];
                    seq_t prev_k = (k <= m - 1) ? tri[offset_prev + k] : (seq_t)0;
                    tri[idx] = prev_k_minus_1 + (seq_t)(m - 1) * prev_k;
                }
            }
        }
        current_max_n = n;
    }

    /* Compute the sequence value a(n) as the required sum */
    seq_t result = (seq_t)0;
    idx_t max_k = n / 3;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = n - 2 * k;
        size_t idx_mk = (size_t)m * (m + 1) / 2 + k;
        result += tri[idx_mk];
    }
    return result;
}
