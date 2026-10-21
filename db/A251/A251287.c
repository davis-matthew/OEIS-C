/*
 * A251287  Number of (n+1)X(3+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge or main diagonally.
 * https://oeis.org/A251287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251287:  Number of (n+1)×(4) 0..1 arrays with every 2×2 subblock
 *  having a single 1 or two 1s on the same edge or main diagonally.
 *
 *  Recurrence (empirical):
 *      a(n) = 2*a(n-1) + 17*a(n-2) - 25*a(n-3) - 74*a(n-4)
 *             + 87*a(n-5) + 108*a(n-6) - 77*a(n-7) - 72*a(n-8)
 *             + 15*a(n-9) + 18*a(n-10) + 2*a(n-11)
 *  Initial values (OEIS start):
 *      a(1)=52, a(2)=219, a(3)=822, a(4)=3390, a(5)=12954,
 *      a(6)=52474, a(7)=202882, a(8)=813336, a(9)=3168621,
 *      a(10)=12623928, a(11)=49416048
 *
 *  This implementation uses a memoised linear‑time algorithm that
 *  works for any seq_t (unsigned integer or arbitrary‑precision
 *  class) and any n>=1.  It avoids illegal casts and uses only
 *  operators allowed on seq_t by the specification.
 */

#include <stdint.h>   /* for idx_t types */
#include <stddef.h>   /* for size_t */
#include "../utils/ensure_capacity.h"

#define ORDER 11

static const int coeff[ORDER] = {
    2,  17,  -25,  -74,
   87, 108,  -77,  -72,
   15,  18,   2
};

/*  Initial terms (a(1) to a(11))  */
static const seq_t init[ORDER] = {
    52ULL,      /* a(1) */
    219ULL,     /* a(2) */
    822ULL,     /* a(3) */
    3390ULL,    /* a(4) */
    12954ULL,   /* a(5) */
    52474ULL,   /* a(6) */
    202882ULL,  /* a(7) */
    813336ULL,  /* a(8) */
    3168621ULL, /* a(9) */
    12623928ULL,/* a(10) */
    49416048ULL /* a(11) */
};

seq_t a(idx_t n)
{
    /*  Memoisation storage: cache[i] == a(i+1)  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* capacity of cache array */

    /*  Initialise cache with the 11 known terms on first call  */
    if (cache_len == 0) {
        ensure_capacity(ORDER, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = 0; i < ORDER; ++i)
            cache[i] = init[i];
        cache_len = ORDER;
    }

    /*  If n is already computed, return it directly  */
    if (n <= cache_len)
        return cache[n - 1];

    /*  Grow cache to hold terms up to n  */
    if (n > cache_cap)
        ensure_capacity(n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /*  Compute missing terms iteratively  */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        /*  a(i+1) depends on a(i), a(i-1), …, a(i-ORDER+1)  */
        for (int k = 0; k < ORDER; ++k) {
            int c = coeff[k];
            seq_t val = cache[(idx_t)(i - k) - 1];   /* term a(i-k)  */
            seq_t prod = val * (seq_t)(c >= 0 ? c : -c);
            if (c >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        cache[i] = sum_pos - sum_neg;   /* result is non‑negative by definition */
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
