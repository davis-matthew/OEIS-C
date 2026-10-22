/*
 * A395002  Number of index 4*n subgroups of SL_2(Z) not containing the
 *          negative identity matrix.
 * https://oeis.org/A395002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + 4*n*a(n-3) + Sum_{i = 1 .. n-4} a(i)*a(n-3-i)
 *            Initial values: a(1)=1, a(2)=8, a(3)=22, a(4)=32 (Derived in
 *            OEIS entry A395002)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of computed terms, indices 1..size   */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated capacity */
    static size_t size    = 0;   /* highest computed index */

    /* undefined for n==0; we return 0 (not used in OEIS offset) */
    if (n == 0) return 0;

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        /* cast required for C++ to accept void* from realloc */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure – terminate */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values */
    for (idx_t k = size + 1; k <= n; ++k) {
        seq_t val = 0;
        if (k == 1) {
            val = 1;
        } else if (k == 2) {
            val = 8;
        } else if (k == 3) {
            val = 22;
        } else if (k == 4) {
            val = 32;
        } else {
            /* 2*a(k-2) */
            val = 2 * cache[k - 2];
            /* 4*k*a(k-3) */
            val += ((seq_t)4 * (seq_t)k) * cache[k - 3];
            /* convolution term */
            for (idx_t i = 1; i <= k - 4; ++i) {
                val += cache[i] * cache[k - 3 - i];
            }
        }
        cache[k] = val;
    }
    size = (size_t)n;
    return cache[n];
}
