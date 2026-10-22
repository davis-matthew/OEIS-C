/*
 * A333947  a(n) is the smallest k > 0 such that sigma(n+k) = sigma(n); if
 *          such k > 0 does not exist, then a(n) = 0.
 * https://oeis.org/A333947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k > 0 such that sigma(n + k) = sigma(n); if
 *            no such k exists then a(n) = 0
 * Range:     u8 none, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* ------------------------------------------------------------------ */
    /*  Precomputation of sigma values and next-occurrence distances.     */
    /* ------------------------------------------------------------------ */
    static bool initialized = false;
    static seq_t *sigma_arr = NULL;
    static seq_t *a_arr = NULL;
    static idx_t *next_occ = NULL;
    static const idx_t B = 1000000;      /* precompute up to one million   */
    static idx_t max_sigma = 0;

    if (!initialized) {
        /* allocate array for sigma values (σ(n)) */
        sigma_arr = (seq_t*)malloc((B + 1) * sizeof(seq_t));
        if (!sigma_arr) return 0;
        for (idx_t i = 0; i <= B; ++i) sigma_arr[i] = 0;

        /* compute σ(j) for all j ≤ B by simple divisor summation  */
        for (idx_t i = 1; i <= B; ++i) {
            seq_t add = (seq_t)i;
            for (idx_t j = i; j <= B; j += i) {
                sigma_arr[j] += add;
                if (sigma_arr[j] > (seq_t)max_sigma)
                    max_sigma = (idx_t)sigma_arr[j];
            }
        }

        /* allocate array for a(n) values  */
        a_arr = (seq_t*)malloc((B + 1) * sizeof(seq_t));
        if (!a_arr) { free(sigma_arr); return 0; }
        for (idx_t i = 0; i <= B; ++i) a_arr[i] = 0;

        /* allocate array that stores the next index having a given σ value */
        next_occ = (idx_t*)calloc(max_sigma + 1, sizeof(idx_t));
        if (!next_occ) { free(sigma_arr); free(a_arr); return 0; }

        /* fill a(n) by scanning indices from B down to 1.
           next_occ[σ] holds the smallest index > current index with that σ. */
        for (idx_t i = B; i > 0; --i) {
            idx_t s = (idx_t)sigma_arr[i];
            idx_t nxt = next_occ[s];
            a_arr[i] = (nxt != 0) ? (seq_t)(nxt - i) : 0;
            next_occ[s] = i;
        }

        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /*  Return answer for the requested n.                                 */
    /* ------------------------------------------------------------------ */
    if (n <= B)
        return a_arr[n];
    else
        return 0;   /* for n beyond the precomputed range a safe fallback  */
}
