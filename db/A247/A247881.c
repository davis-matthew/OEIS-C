/*
 * A247881  Numbers of the form x^2 + 13*y^2.
 * https://oeis.org/A247881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth non‑negative integer of the form x^2 + 13*y^2 *
 *            (with x,y ≥ 0). The sequence starts with a(1)=0. The function *
 *            generates all such integers up to a certain bound, stores them
 *            in a * static cache, and returns the element at the requested
 *            index. * * The implementation keeps a dynami...
 * Range:     u8 n <= 58, u16 n <= 9059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- helpers ---------- */

/* Integer square root: floor(sqrt(n)) for an unsigned 64‑bit integer */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* ---------- cache storage ---------- */

static seq_t *cache = NULL;          /* array of already computed terms */
static size_t cache_len = 0;          /* number of terms stored */

/* Generates the cache so that it holds at least `wanted` terms.
 * If the cache is already large enough nothing is done. */
static void generate_cache(size_t wanted)
{
    if (cache_len >= wanted)
        return;                       /* nothing to do */

    /* initial bound guess – grow until enough terms appear */
    size_t M = (wanted > 200) ? wanted * 4 : 400;   /* initial bound */

    for (;;) {
        /* allocate bitset: one byte per candidate value up to M */
        uint8_t *present = (uint8_t *)calloc(M + 1, 1);
        if (!present)
            return;   /* allocation failure – fallback behaviour is undefined */

        /* enumerate all representations x^2 + 13*y^2 ≤ M */
        for (uint64_t y = 0; ; ++y) {
            __uint128_t y2 = (__uint128_t)13 * y * y;
            if (y2 > M)
                break;
            uint64_t max_x = isqrt_u64((uint64_t)(M - y2));
            for (uint64_t x = 0; x <= max_x; ++x) {
                uint64_t val = (uint64_t)(x * x + y2);
                present[val] = 1;
            }
        }

        /* count the distinct values and find how many there are */
        size_t new_len = 0;
        for (size_t i = 0; i <= M; ++i)
            if (present[i])
                ++new_len;

        if (new_len >= wanted) {                  /* enough terms – keep them */
            seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!new_cache) {                      /* allocation failure */
                free(present);
                return;
            }

            size_t idx = 0;
            for (size_t i = 0; i <= M; ++i)
                if (present[i]) {
                    new_cache[idx++] = (seq_t)i;
                }

            free(present);
            free(cache);                            /* free old cache */
            cache = new_cache;
            cache_len = new_len;
            return;
        }

        /* not enough terms – double the bound and try again */
        free(present);
        M *= 2;
    }
}

/* ---------- required function ---------- */

seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1)=0.  The caller guarantees n≥1. */
    if (n < (idx_t)1)       /* defensive: return 0 for invalid indices */
        return (seq_t)0;

    generate_cache((size_t)n);
    return cache[(size_t)(n - 1)];
}
