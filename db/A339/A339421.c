/*
 * A339421  Number of compositions (ordered partitions) of n into an odd
 *          number of cubes.
 * https://oeis.org/A339421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into an odd number of cubes.
 *            Recurrence: E[0] = 1, O[0] = 0 O[n] = Σ_{k≥1, k³≤n} E[n - k³]
 *            E[n] = Σ_{k≥1, k³≤n} O[n - k³] The function returns O[n] for
 *            every n ≥ 0
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 112, u64 n <= 218,
 *            u128 n <= 429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* static cache of E (even) and O (odd) composition counts        */
    static seq_t *odd   = NULL;   /* O[n] */
    static seq_t *even  = NULL;   /* E[n] */
    static idx_t  size  = 0;      /* allocated array size (>= computed + 1) */
    static idx_t  computed = 0;   /* largest n for which values are computed */

    /* Ensure the cache is initialised                      */
    if (!odd || !even) {
        size = n + 1;
#ifdef __cplusplus
        odd  = new seq_t[size];
        even = new seq_t[size];
#else
        odd  = (seq_t*)malloc(size * sizeof(seq_t));
        even = (seq_t*)malloc(size * sizeof(seq_t));
#endif
        if (!odd || !even) return 0;   /* allocation failure */
        odd[0]  = (seq_t)0;            /* O[0] = 0 */
        even[0] = (seq_t)1;            /* E[0] = 1 */
        computed = 0;
    }

    /* Grow the cache if the requested n is larger than current capacity */
    if (n + 1 > size) {
        idx_t new_size = n + 1;
        seq_t *odd_new;
        seq_t *even_new;
#ifdef __cplusplus
        odd_new  = new seq_t[new_size];
        even_new = new seq_t[new_size];
#else
        odd_new  = (seq_t*)malloc(new_size * sizeof(seq_t));
        even_new = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        if (!odd_new || !even_new) return 0;
        /* copy old values */
        for (idx_t i = 0; i <= computed; ++i) {
            odd_new[i]  = odd[i];
            even_new[i] = even[i];
        }
        /* free old arrays */
#ifdef __cplusplus
        delete [] odd;
        delete [] even;
#else
        free(odd);
        free(even);
#endif
        odd = odd_new;
        even = even_new;
        size = new_size;
    }

    /* Compute new values up to n if necessary */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum_odd  = 0;   /* value for O[i]   */
            seq_t sum_even = 0;   /* value for E[i]   */
            for (idx_t k = 1; ; ++k) {
                idx_t cube = k * k * k;
                if (cube > i) break;
                sum_odd  += even[i - cube];
                sum_even += odd[i - cube];
            }
            odd[i]  = sum_odd;
            even[i] = sum_even;
        }
        computed = n;
    }

    return odd[n];
}
