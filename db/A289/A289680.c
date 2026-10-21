/*
 * A289680  a(n) = Catalan(n+1)*Bell(n).
 * https://oeis.org/A289680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n+1) * Bell(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* --- static cache for Bell numbers -------------------------------- */
    static seq_t *bell_cache = NULL;   /* bell_cache[k] = Bell(k)          */
    static size_t bell_cap   = 0;       /* capacity of bell_cache           */
    static seq_t *prev_row   = NULL;   /* previous row of Bell triangle    */
    static size_t prev_len   = 0;       /* length of prev_row               */
    static idx_t  bell_max   = 0;       /* highest n for which Bell is known */

    /* --- initialise on first call ------------------------------------- */
    if (bell_cache == NULL) {
        bell_cap = 1;
        bell_cache = (seq_t*)malloc(sizeof(seq_t) * bell_cap);
        bell_cache[0] = (seq_t)1;          /* Bell(0) = 1 */

        prev_len   = 1;
        prev_row   = (seq_t*)malloc(sizeof(seq_t) * prev_len);
        prev_row[0] = (seq_t)1;            /* first Bell triangle row       */

        bell_max = 0;
    }

    /* --- grow cache array if necessary --------------------------------- */
    if ((idx_t)n >= (idx_t)bell_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(bell_cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* allocation failed – abort (not expected in tests) */
            abort();
        }
        bell_cache = tmp;
        bell_cap   = new_cap;
    }

    /* --- compute Bell numbers up to n if not computed yet -------------- */
    if (n > bell_max) {
        for (idx_t k = bell_max + 1; k <= n; ++k) {
            size_t cur_len = (size_t)k + 1;
            seq_t *cur_row = (seq_t*)malloc(sizeof(seq_t) * cur_len);
            if (!cur_row) abort();

            /* first entry of the new row is the last entry of the previous row */
            cur_row[0] = prev_row[prev_len - 1];

            /* remaining entries of the row */
            for (size_t j = 1; j < cur_len; ++j) {
                cur_row[j] = cur_row[j - 1] + prev_row[j - 1];
            }

            /* store Bell(k) */
            bell_cache[k] = cur_row[0];

            /* advance to next row */
            free(prev_row);
            prev_row = cur_row;
            prev_len = cur_len;
            bell_max = k;
        }
    }

    /* --- compute the required term ----------------------------------- */
    seq_t cat  = catalan((idx_t)n + 1);   /* Catalan(n+1) */
    seq_t bell = bell_cache[n];          /* Bell(n)     */
    return cat * bell;
}
