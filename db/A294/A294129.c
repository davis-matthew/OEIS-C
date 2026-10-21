/*
 * A294129  Numbers n for which exactly one length minimal language exists
 *          having exactly n nonempty words over a countably infinite alphabet
 *          such that within each prefix of a word every letter of the
 *          alphabet is at least as frequent as the subsequent alphabet
 *          letter.
 * https://oeis.org/A294129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + A000085(n-1), a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;
    static seq_t *inv_vals = NULL;
    static idx_t max_n = 0;

    if (n == 0)          /* below offset: return 0 (unused) */
        return 0;

    if (max_n < n) {
        /* Allocate space for indices 0 .. n */
        idx_t new_cap = n + 1;
        a_vals   = (seq_t*)realloc(a_vals,   new_cap * sizeof(seq_t));
        inv_vals = (seq_t*)realloc(inv_vals, new_cap * sizeof(seq_t));

        /* Initial values for involutions */
        if (max_n == 0) {
            inv_vals[0] = 1;  /* f(0) = 1 for involutions */
        }

        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1) {
                /* A000085(1) = 1, a(1) = 0 */
                inv_vals[i] = 1;
                a_vals[i]   = 0;
            } else {
                /* Compute involutions: f(i) = f(i-1) + (i-1)*f(i-2) */
                inv_vals[i] = inv_vals[i-1] + (seq_t)(i-1) * inv_vals[i-2];
                /* Compute a: a(i) = a(i-1) + A000085(i-1) */
                a_vals[i] = a_vals[i-1] + inv_vals[i-1];
            }
        }
        max_n = n;
    }

    return a_vals[n];
}
