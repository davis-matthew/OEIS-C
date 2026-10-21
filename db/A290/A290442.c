/*
 * A290442  a(n) = Catalan(n-1)*Motzkin(n).
 * https://oeis.org/A290442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n-1) * Motzkin(n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a sequence that uses the recurrence
   n*(n+2)*a(n) -2*(2*n+1)*(2*n-3)*a(n-1) -12*(2*n-3)*(2*n-5)*a(n-2) =0.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cat = NULL;      /* Catalan numbers: cat[k] = C(k) */
    static idx_t cat_size = 0;     /* number of allocated entries (0..cat_size-1) */

    static seq_t *mot = NULL;      /* Motzkin numbers: mot[k] = M(k) */
    static idx_t mot_size = 0;     /* number of allocated entries (0..mot_size-1) */

    if (n < 1) {
        return (seq_t)0;           /* undefined for n < 1 */
    }

    /* Ensure Catalan numbers up to C(n-1) */
    if (cat_size < n) {
        idx_t new_size = n;        /* need entries 0 .. n-1 */
        seq_t *tmp = (seq_t *)realloc(cat, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cat = tmp;
        if (cat_size == 0) {
            cat[0] = (seq_t)1;    /* C(0) = 1 */
            cat_size = 1;
        }
        for (idx_t k = cat_size; k < n; ++k) {
            idx_t prev = k - 1;
            seq_t num = cat[prev] * (seq_t)(4 * prev + 2);
            seq_t val = num / (seq_t)(prev + 2);  /* division exact by construction */
            cat[k] = val;
        }
        cat_size = n;            /* now cat[0 .. n-1] are valid */
    }

    /* Ensure Motzkin numbers up to M(n) */
    if (mot_size < n + 1) {
        idx_t new_size = n + 1;  /* need entries 0 .. n */
        seq_t *tmp = (seq_t *)realloc(mot, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        mot = tmp;
        if (mot_size == 0) {
            mot[0] = (seq_t)1;  /* M(0) = 1 */
            if (new_size > 1) {
                mot[1] = (seq_t)1;  /* M(1) = 1 */
            }
            mot_size = (new_size > 1) ? 2 : 1;
        }
        for (idx_t k = mot_size; k <= n; ++k) {
            if (k <= 1) continue; /* M(0) and M(1) already set */
            seq_t term1 = mot[k - 1] * (seq_t)(2 * k + 1);
            seq_t term2 = mot[k - 2] * (seq_t)(3 * k - 3);
            seq_t num  = term1 + term2;
            seq_t val  = num / (seq_t)(k + 2);  /* division exact by construction */
            mot[k] = val;
        }
        mot_size = n + 1;          /* now mot[0 .. n] are valid */
    }

    /* Compute a(n) = C(n-1) * M(n) */
    return cat[n - 1] * mot[n];
}
