/*
 * A251232  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 6 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -63*a(n-2) +122*a(n-3) +13*a( n-4) -371*a(n-5)
 *            +317*a(n-6) +308*a(n-7) -370*a(n-8) -144*a(n-9) +128*a(n-10)
 *            +48*a(n-11) Initial values: a(1)=966, a(2)=1870, a(3)=3886,
 *            a(4)=8686, a(5)=20490, a(6)=50570, a(7)=129378, a(8)=340782,
 *            a(9)=918878, a(10)=2525262, a(11)=7049658
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   mul_abs_by_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_abs_by_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* recurrence coefficients (length 11) */
    static const int coeffs[11] = {
        13, -63, 122, 13, -371, 317, 308, -370, -144, 128, 48
    };

    /* base values a(1)..a(11) */
    static const seq_t base[12] = {
        0,    /* unused index 0 */
        966ULL, 1870ULL, 3886ULL, 8686ULL, 20490ULL, 50570ULL,
        129378ULL, 340782ULL, 918878ULL, 2525262ULL, 7049658ULL
    };

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t  computed_upto = 0;

    if (computed_upto == 0) {         /* first call: initialise cache */
        cache_size = 12;              /* indices 0..11 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;         /* allocation failure */

        for (idx_t i = 0; i <= 11; ++i)
            cache[i] = base[i];

        computed_upto = 11;
    }

    if (n <= computed_upto)
        return cache[n];

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t cur = computed_upto + 1; cur <= n; ++cur) {
        if (cur <= 11) {              /* we already have these */
            cache[cur] = base[cur];
            continue;
        }

        seq_t pos = 0;
        seq_t neg = 0;

        for (int j = 0; j < 11; ++j) {
            idx_t idx_prev = cur - (j + 1);   /* a(cur-(j+1)) */
            seq_t term = cache[idx_prev];
            int c = coeffs[j];
            seq_t prod = mul_abs_by_uint(term, (unsigned int)(c >= 0 ? c : -c));
            if (c >= 0)
                pos += prod;
            else
                neg += prod;
        }

        cache[cur] = pos - neg;
    }

    computed_upto = n;
    return cache[n];
}
