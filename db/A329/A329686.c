/*
 * A329686  Number of excursions of length n with Motzkin-steps forbidding all
 *          consecutive steps of length 2 except UH, HU, HD and DH.
 * https://oeis.org/A329686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   * a(0) = 1 * a(4n) = 2 * Catalan(n) (n ≥ 1) * a(4n-1) =
 *            Catalan(n) * a(4n+1) = Catalan(n) * a(4n+2) = 0 * (Catalan(0)=1,
 *            Catalan(n+1)=Catalan(n)*2*(2n+1)/(n+2)) * * The sequence is thus
 *            computed via the closed‑form with Catalan numbers, * which is
 *            O(sqrt(n)) precomputation for all required values
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cat_cache = NULL;         /* array storing Catalan numbers C(0)...C(k) */
static size_t cat_cache_size = 0;       /* number of elements currently stored */

/* Ensure Catalan numbers up to index k are computed and cached.
 * Return C(k). */
static seq_t get_cat(idx_t k)
{
    if (cat_cache == NULL) {
        cat_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cat_cache) return (seq_t)0;           /* allocation failure – unlikely */
        cat_cache[0] = (seq_t)1;                 /* C(0) = 1 */
        cat_cache_size = 1;
    }
    if (cat_cache_size <= k) {
        size_t new_size = (size_t)(k + 1);
        seq_t *tmp = (seq_t*)realloc(cat_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failure – unlikely */
        cat_cache = tmp;
        for (size_t i = cat_cache_size; i < new_size; ++i) {
            idx_t idx = (idx_t)i;
            seq_t prev = cat_cache[i - 1];
            seq_t num  = prev * (seq_t)(4 * idx - 2);
            seq_t denom = (seq_t)(idx + 1);
            cat_cache[i] = num / denom;            /* exact division */
        }
        cat_cache_size = new_size;
    }
    return cat_cache[k];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                 /* special initial term */

    idx_t r = n % 4;
    idx_t m = n / 4;

    switch (r) {
    case 0:                                        /* a(4n) = 2 * C(n) */
        return get_cat(m) * (seq_t)2;
    case 1:                                        /* a(4n+1) = C(n) */
        return get_cat(m);
    case 2:                                        /* a(4n+2) = 0 */
        return (seq_t)0;
    case 3:                                        /* a(4n-1) = C(n+1) */
        return get_cat(m + 1);
    default:
        return (seq_t)0;                           /* never reached */
    }
}
