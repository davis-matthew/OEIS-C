/*
 * A307555  Number of Motzkin meanders of length n with an even number of
 *          humps.
 * https://oeis.org/A307555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307555: Number of Motzkin meanders of length n with an even number
 * of humps.
 *
 * The sequence satisfies the 8‑th order recurrence
 *   -3*(n+1)*(n-2)*a(n)
 *   +12*(2*n^2-4*n-1)*a(n-1)
 *   +2*(-35*n^2+107*n-48)*a(n-2)
 *   +4*(21*n^2-89*n+80)*a(n-3)
 *   +4*(-5*n^2+32*n-43)*a(n-4)
 *   +4*(-8*n^2+62*n-115)*a(n-5)
 *   +2*(31*n^2-283*n+616)*a(n-6)
 *   -4*(23*n-97)*(n-6)*a(n-7)
 *   +15*(n-6)*(n-7)*a(n-8)
 *   = 0
 * with initial values a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=17,
 * a(5)=40, a(6)=106, a(7)=307, a(8)=927.
 *
 * The implementation uses memoisation and proceeds in linear time.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* memoised a(n) values */
static idx_t  cached_up_to = 0;      /* highest n whose a(n) is known */

/* initialise cache with the first nine terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache = (seq_t*)calloc(9, sizeof(seq_t));
    if (cache == NULL)
        abort();

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)17;
    cache[5] = (seq_t)40;
    cache[6] = (seq_t)106;
    cache[7] = (seq_t)307;
    cache[8] = (seq_t)927;
    cached_up_to = 8;
}

/* grow cache array to hold values up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL)
        init_cache();

    if (cached_up_to >= n)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL)
        abort();
    cache = new_cache;

    /* compute missing values */
    for (idx_t k = cached_up_to + 1; k <= n; ++k)
    {
        /* coefficients (may be negative) */
        int64_t kk = (int64_t)k;           /* signed for coefficient calculation */
        int64_t c1 = 12 * (2*kk*kk - 4*kk - 1);
        int64_t c2 = 2  * (-35*kk*kk + 107*kk - 48);
        int64_t c3 = 4  * (21*kk*kk - 89*kk + 80);
        int64_t c4 = 4  * (-5*kk*kk + 32*kk - 43);
        int64_t c5 = 4  * (-8*kk*kk + 62*kk - 115);
        int64_t c6 = 2  * (31*kk*kk - 283*kk + 616);
        int64_t c7 = -4 * (23*kk - 97) * (kk - 6);
        int64_t c8 = 15 * (kk - 6) * (kk - 7);

        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        /* helper to process a term */
        #define ADD_TERM(idx, coeff)                                        \
            do {                                                            \
                seq_t prod = cache[(idx)] * (seq_t)((coeff) >= 0 ? (coeff) : -(coeff)); \
                if ((coeff) >= 0)                                           \
                    pos_sum += prod;                                        \
                else                                                        \
                    neg_sum += prod;                                        \
            } while (0)

        ADD_TERM(k-1, c1);
        ADD_TERM(k-2, c2);
        ADD_TERM(k-3, c3);
        ADD_TERM(k-4, c4);
        ADD_TERM(k-5, c5);
        ADD_TERM(k-6, c6);
        ADD_TERM(k-7, c7);
        ADD_TERM(k-8, c8);

        /* sum of the signed contributions */
        seq_t rhs = pos_sum - neg_sum;
        seq_t denom = (seq_t)(3 * (kk + 1) * (kk - 2));

        cache[k] = rhs / denom;
    }

    cached_up_to = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ensure the cache is initialised before any access */
    if (cache == NULL)
        init_cache();

    if (n <= cached_up_to)
        return cache[n];

    ensure_cache_size(n);
    return cache[n];
}
