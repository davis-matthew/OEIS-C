/*
 * A269639  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the order‑16 linear recurrence 56·a(n‑1) –
 *            1344·a(n‑2) + 17752·a(n‑3) – 135716·a(n‑4) + 545496·a(n‑5) –
 *            463786·a(n‑6) – 4675564·a(n‑7) + 12243456·a(n‑8) +
 *            21822328·a(n‑9) – 72836660·a(n‑10) – 123804240·a(n‑11) +
 *            151424498·a(n‑12) + 520495760·a(n‑13) + 515921322·a(n‑14) +
 *            232811740·...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1 .. 16 */
    static const seq_t init[17] = {
        0ULL,               /* a(0) unused */
        8ULL,
        64ULL,
        504ULL,
        3933ULL,
        30444ULL,
        234080ULL,
        1789528ULL,
        13613507ULL,
        103118640ULL,
        778158768ULL,
        5852649288ULL,
        43888314093ULL,
        328240366020ULL,
        2449045039552ULL,
        18233174801384ULL,
        135479775215950ULL
    };

    if (n <= 16)
        return init[(size_t)n];

    /* cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t computed_up_to = 16;

    /* ensure cache is large enough */
    size_t need = (size_t)n + 1;
    if (need > cache_len) {
        size_t new_len = need > cache_len * 2 ? need : cache_len * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure – unlikely */
        cache = new_cache;
        cache_len = new_len;
        /* initialize base values if cache was just created */
        if (cache_len >= 17) {
            for (size_t i = 0; i <= 16; ++i)
                cache[i] = init[i];
        }
    }

    /* recurrence coefficients (order 16) */
    static const long long coeffs[16] = {
        56LL, -1344LL, 17752LL, -135716LL,
        545496LL, -463786LL, -4675564LL, 12243456LL,
        21822328LL, -72836660LL, -123804240LL, 151424498LL,
        520495760LL, 515921322LL, 232811740LL, 41061559LL
    };

    /* compute terms up to n if needed */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int k = 0; k < 16; ++k) {
            idx_t idx = i - (k + 1);
            /* absolute value of coefficient */
            seq_t abscoeff = (seq_t)(coeffs[k] > 0 ? coeffs[k] : -coeffs[k]);
            seq_t term = cache[idx] * abscoeff;
            if (coeffs[k] > 0)
                sum_pos += term;
            else
                sum_neg += term;
        }
        cache[i] = sum_pos - sum_neg;
    }
    computed_up_to = n;

    return cache[n];
}
