/*
 * A334821  Number of integers from 0 to the triangular number A000217(n) that
 *          can be represented as a difference of triangular numbers from 0 to
 *          n.
 * https://oeis.org/A334821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers in [0, T(n)] that are differences of
 *            two triangular numbers ≤ n. Computed by dynamically building the
 *            set of representable differences
 * Range:     u8 n <= 22, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static state */
    static idx_t max_n = (idx_t)-1;        /* highest n already processed */
    static seq_t *tri = NULL;             /* triangular numbers (0..max_n) */
    static size_t tri_len = 0, tri_cap = 0;
    static uint8_t *mask = NULL;          /* bitset of representable numbers */
    static size_t mask_bytes = 0;
    static seq_t *a_cache = NULL;         /* cached a(k) values */
    static size_t a_cap = 0;
    static seq_t count = 0;               /* current number of representable numbers */

    /* --- initialization for n = 0 -------------------------------------------------- */
    if (max_n == (idx_t)-1) {
        /* just one triangular number: 0 */
        tri_cap = 1;
        tri = (seq_t *)malloc(tri_cap * sizeof(seq_t));
        tri[0] = 0;
        tri_len = 1;

        /* mask for numbers 0..0 (1 bit) */
        mask_bytes = 1;
        mask = (uint8_t *)calloc(mask_bytes, 1);
        mask[0] = 1;   /* set bit 0 */
        count = 1;

        /* cache for a(0) */
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = count;
        max_n = 0;
    }

    /* --- extend computation up to n if necessary --------------------------------- */
    if (n > max_n) {
        /* ensure cache capacity */
        if (a_cap <= (size_t)n) {
            size_t new_cap = a_cap ? a_cap : 1;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            a_cap = new_cap;
        }

        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* compute T_k = k*(k+1)/2 using previous triangular number */
            seq_t T_k = (tri_len > 0) ? (tri[tri_len - 1] + k) : 0;

            /* ensure capacity for triangular numbers */
            if (tri_len == tri_cap) {
                size_t new_cap = tri_cap ? tri_cap * 2 : 1;
                tri = (seq_t *)realloc(tri, new_cap * sizeof(seq_t));
                tri_cap = new_cap;
            }
            tri[tri_len++] = T_k;

            /* ensure mask size for up to T_k */
            size_t need_bytes = (((size_t)T_k) + 7) / 8 + 1;
            if (need_bytes > mask_bytes) {
                mask = (uint8_t *)realloc(mask, need_bytes);
                memset(mask + mask_bytes, 0, need_bytes - mask_bytes);
                mask_bytes = need_bytes;
            }

            /* add all new differences T_k - T_j (j = 0..k) */
            for (idx_t j = 0; j <= k; ++j) {
                seq_t diff = T_k - tri[j];
                size_t idx = (size_t)diff;
                size_t byte = idx >> 3;
                uint8_t bit = 1U << (idx & 7);
                if (!(mask[byte] & bit)) {
                    mask[byte] |= bit;
                    ++count;
                }
            }

            /* cache a(k) */
            a_cache[k] = count;
        }
        max_n = n;
    }

    return a_cache[n];
}
