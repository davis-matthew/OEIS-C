/*
 * A352591  Number of tilings of a 5 X n rectangle using 2 X 2 and 1 X 1 tiles
 *          and dominoes.
 * https://oeis.org/A352591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) + 361*a(n-2) - 200*a(n-3) - 17183*a(n-4) +
 *            6963*a(n-5) + 320541*a(n-6) - 363476*a(n-7) - 2096695*a(n-8) +
 *            2659007*a(n-9) + 6914929*a(n-10) - 7710612*a(n-11) -
 *            13172634*a(n-12) + 10195340*a(n-13) + 14247416*a(n-14) -
 *            5770992*a(n-15) - 7493472*a(n-16) + 1420800*a(n-17) + 1 with
 *            a(0...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   parse_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/parse_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;
    static bool big_inited = false;
    static seq_t big14, big15, big16, big17;

    /* First call: allocate and initialise cache. */
    if (!cache) {
        idx_t cap = n + 1;
        if (cap < 18) cap = 18;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_size = cap;

        /* Small initial constants that fit in 64 bits. */
        const seq_t init_vals[14] = {
            1ULL,
            8ULL,
            306ULL,
            7546ULL,
            210690ULL,
            5684228ULL,
            154869092ULL,
            4207660108ULL,
            114411435032ULL,
            3110251075956ULL,
            84557403666284ULL,
            2298788023809188ULL,
            62495515282157930ULL,
            1699018055917569318ULL
        };
        for (idx_t i = 0; i < 14; ++i) cache[i] = init_vals[i];

        if (!big_inited) {
            big14 = parse_big("46189937030173640586");
            big15 = parse_big("1255731173553810109440");
            big16 = parse_big("34138623221740999081824");
            big17 = parse_big("928101175618008434398704");
            big_inited = true;
        }
        cache[14] = big14;
        cache[15] = big15;
        cache[16] = big16;
        cache[17] = big17;
        computed = 18;
    }

    /* Ensure sufficient capacity. */
    if (n + 1 > cache_size) {
        idx_t new_cap = cache_size;
        while (n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            /* Positive part of the recurrence. */
            seq_t pos = cache[i - 1] * 15;
            pos += cache[i - 2] * 361;
            pos += cache[i - 5] * 6963;
            pos += cache[i - 6] * 320541;
            pos += cache[i - 9] * 2659007;
            pos += cache[i - 10] * 6914929;
            pos += cache[i - 13] * 10195340;
            pos += cache[i - 14] * 14247416;
            pos += cache[i - 17] * 1420800;
            pos += 1;

            /* Negative part of the recurrence. */
            seq_t neg = cache[i - 3] * 200;
            neg += cache[i - 4] * 17183;
            neg += cache[i - 7] * 363476;
            neg += cache[i - 8] * 2096695;
            neg += cache[i - 11] * 7710612;
            neg += cache[i - 12] * 13172634;
            neg += cache[i - 15] * 5770992;
            neg += cache[i - 16] * 7493472;

            cache[i] = pos - neg;
        }
        computed = n + 1;
    }

    return cache[n];
}
