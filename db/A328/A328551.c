/*
 * A328551  a(n) is the Severi degree for curves of degree n and cogenus 4.
 * https://oeis.org/A328551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            11, * with a(1)=a(2)=a(3)=0, a(4)=666, a(5)=36975, a(6)=437517,
 *            * a(7)=2667375, a(8)=11225145, a(9)=37206936, a(10)=104285790, *
 *            a(11)=257991042. For 3 < n ≤ 11 the values are g...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 220, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef __int128 s128;
#else
typedef long long s128;
#endif

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Compute a(n) for 3 ≤ n ≤ 11 using the polynomial formula */
static seq_t compute_small(idx_t n)
{
    s128 nn = (s128)n;
    s128 num = -70920
             + 36114 * nn
             + 37881 * nn * nn
             - 20232 * nn * nn * nn
             - 5136 * nn * nn * nn * nn
             + 3618 * nn * nn * nn * nn * nn
             - 216  * nn * nn * nn * nn * nn * nn * nn
             + 27   * nn * nn * nn * nn * nn * nn * nn * nn;
    return (seq_t)(num / 8);
}

/* Initialise cache with a(1) … a(11) */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_size = 11;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (cache == NULL)
        return;          /* allocation failure – unlikely in test harness */

    cache[0] = 0;                /* a(1) */
    cache[1] = 0;                /* a(2) */
    cache[2] = 0;                /* a(3) */
    cache[3] = compute_small(4);
    cache[4] = compute_small(5);
    cache[5] = compute_small(6);
    cache[6] = compute_small(7);
    cache[7] = compute_small(8);
    cache[8] = compute_small(9);
    cache[9] = compute_small(10);
    cache[10] = compute_small(11);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset is 1, but protect against 0 */

    init_cache();

    size_t idx = (size_t)n;

    /* Return a precomputed value if available */
    if (idx <= cache_size)
        return cache[idx - 1];

    /* Grow cache up to needed size */
    size_t new_size = idx;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        return 0;          /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence */
    for (size_t i = cache_size + 1; i <= new_size; ++i) {
        seq_t sum = cache[i - 2];
        sum *= 9;

        seq_t tmp_term = cache[i - 3];
        tmp_term *= 36;
        sum -= tmp_term;

        tmp_term = cache[i - 4];
        tmp_term *= 84;
        sum += tmp_term;

        tmp_term = cache[i - 5];
        tmp_term *= 126;
        sum -= tmp_term;

        tmp_term = cache[i - 6];
        tmp_term *= 126;
        sum += tmp_term;

        tmp_term = cache[i - 7];
        tmp_term *= 84;
        sum -= tmp_term;

        tmp_term = cache[i - 8];
        tmp_term *= 36;
        sum += tmp_term;

        tmp_term = cache[i - 9];
        tmp_term *= 9;
        sum -= tmp_term;

        tmp_term = cache[i - 10];
        sum += tmp_term;

        cache[i - 1] = sum;
    }

    cache_size = new_size;
    return cache[idx - 1];
}
