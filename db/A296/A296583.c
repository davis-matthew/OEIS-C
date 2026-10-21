/*
 * A296583  Number of nX3 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A296583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + a(n-2) -12*a(n-3) -16*a(n-4) -3*a(n-5)
 *            -12*a(n-6) -52*a(n-7) -173*a(n-8) -97*a(n-9) -25*a(n-10)
 *            -112*a(n-11) -31*a(n-12) -244*a(n-13) +12*a(n-14) +73*a(n-15)
 *            -121*a(n-16) +73*a(n-17) +26*a(n-18) -3*a(n-19) +12*a(n-20)
 *            +4*a(n-21). Initial values: a(1)=5, a(2)=22, a(3)=83, a(4)=34...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

#define MAX_CACHE 20000  /* safe for n up to about 10000 */

/* Initial 21 terms of the sequence */
static const seq_t init_terms[21] = {
    5ull, 22ull, 83ull, 346ull, 1465ull, 6241ull, 26895ull, 116541ull,
    507102ull, 2212212ull, 9668284ull, 42302733ull, 185232946ull,
    811492755ull, 3556247878ull, 15588051447ull, 68336370049ull,
    299606590345ull, 1313640472782ull, 5759946595000ull, 25256394023434ull
};

/* Coefficients of the 21‑term recurrence */
static const int coeffs[21] = {
    5, 1, -12, -16, -3, -12, -52, -173, -97, -25,
    -112, -31, -244, 12, 73, -121, 73, 26, -3, 12, 4
};

/* Cache for computed terms – statically allocated so it works in C and C++ */
static seq_t cache[MAX_CACHE];
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Initialize cache with the first 21 terms once */
    if (cache_size == 0) {
        memcpy(cache, init_terms, 21 * sizeof(seq_t));
        cache_size = 21;
    }

    /* Compute missing terms up to n if necessary */
    if ((size_t)n > cache_size) {
        if ((size_t)n > MAX_CACHE) {
            /* Requested n exceeds the static buffer – stop computation */
            return 0;
        }
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int j = 1; j <= 21; ++j) {
                int c = coeffs[j - 1];
                seq_t val = cache[(size_t)(i - j - 1)];  /* a(i-j) */
                if (c > 0)
                    pos += (seq_t)c * val;
                else
                    neg += (seq_t)(-c) * val;
            }
            cache[(size_t)(i - 1)] = pos - neg;  /* a(i) */
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)(n - 1)];
}
