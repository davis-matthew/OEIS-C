/*
 * A270114  Number of nX4 0..7 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling seven exactly once.
 * https://oeis.org/A270114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   parse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270114:
 * a(n) = 3378*a(n-1) -3070271*a(n-2) +370787212*a(n-3)
 *      -17482148143*a(n-4) +363880874050*a(n-5) -2797694462161*a(n-6)
 * for n > 7, with a(1) … a(7) as initial values.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/parse_decimal.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* undefined below offset */

    /* --- cache of computed values ------------------------------------- */
    static seq_t *cache = NULL;           /* cache[1 .. last]  (index 0 unused) */
    static idx_t cache_len = 0;           /* allocated length (cache_len==0 -> none) */
    static idx_t last_computed = 0;       /* highest index stored in cache */

    /* enlarge cache if necessary */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        /* initialise newly allocated elements to zero */
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = (seq_t)0;
        cache_len = (idx_t)new_len;
    }

    /* --- initial values ---------------------------------------------- */
    static seq_t init_val[8];
    static bool init_done = false;
    if (!init_done) {
        init_val[1] = parse_decimal("1176");
        init_val[2] = parse_decimal("6183744");
        init_val[3] = parse_decimal("16972694928");
        init_val[4] = parse_decimal("38799000374400");
        init_val[5] = parse_decimal("81224231406183600");
        init_val[6] = parse_decimal("161437591343857507392");
        init_val[7] = parse_decimal("310047482243688075320784");
        init_done = true;
    }

    /* --- compute missing terms ---------------------------------------- */
    static const seq_t coeff_pos[3] = {(seq_t)3378ULL,
                                       (seq_t)370787212ULL,
                                       (seq_t)363880874050ULL};
    static const seq_t coeff_neg[3] = {(seq_t)3070271ULL,
                                       (seq_t)17482148143ULL,
                                       (seq_t)2797694462161ULL};

    for (idx_t k = last_computed + 1; k <= n; ++k) {
        if (k <= 7) {
            cache[k] = init_val[(int)k];
        } else {
            /* a(k) = +c1*a(k-1) +c3*a(k-3)+c5*a(k-5)
                     -c2*a(k-2)-c4*a(k-4)-c6*a(k-6) */
            seq_t pos = coeff_pos[0] * cache[k-1]
                      + coeff_pos[1] * cache[k-3]
                      + coeff_pos[2] * cache[k-5];
            seq_t neg = coeff_neg[0] * cache[k-2]
                      + coeff_neg[1] * cache[k-4]
                      + coeff_neg[2] * cache[k-6];
            cache[k] = pos - neg;              /* pos >= neg by definition */
        }
    }
    last_computed = n;

    return cache[n];
}
