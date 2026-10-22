/*
 * A317767  Number of nX2 0..1 arrays with every element unequal to 0, 1 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) - a(n-6) for n > 6 Initial values:
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=18
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 44, u64 n <= 88, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* ---------- C++ implementation ------------------------------------- */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    /* ensure cache has at least (n+1) elements and all needed terms are computed */
    auto ensure_size = [&cache](idx_t target) -> void {
        if (cache.empty()) {
            cache.resize(1);          /* cache[0] unused / set to 0 */
            cache[0] = (seq_t)0;
        }
        if (cache.size() <= target) {
            idx_t old_sz = cache.size() - 1;          /* last valid index */
            cache.resize(target + 1);                /* new elements value‑initialized to 0 */
            for (idx_t i = old_sz + 1; i <= target; ++i) {
                if (i <= 6) {
                    switch (i) {
                        case 1: cache[i] = 2; break;
                        case 2: cache[i] = 3; break;
                        case 3: cache[i] = 4; break;
                        case 4: cache[i] = 7; break;
                        case 5: cache[i] = 11; break;
                        case 6: cache[i] = 18; break;
                    }
                } else {
                    cache[i] = cache[i-1] + ((seq_t)2)*cache[i-3] - cache[i-6];
                }
            }
        }
    };
    ensure_size(n);
    return cache[n];
#else
    /* ---------- C implementation --------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t stored_max = 0;

    if (n == 0)          /* offset is 1; undefined below 1, return 0 */
        return 0;

    if (n > stored_max) {
        idx_t needed = n + 1;          /* we use 1‑based indexing, cache[0] unused */
        if (needed > cap) {
            size_t new_cap = needed > cap * 2 ? needed : cap * 2;
            if (cap == 0) {
                cache = calloc(new_cap, sizeof(seq_t));   /* zero‑initialised */
            } else {
                seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (tmp) {
                    cache = tmp;
                } else {             /* allocation failed – give up */
                    return 0;
                }
            }
            cap = new_cap;
        }

        for (idx_t i = stored_max + 1; i <= n; ++i) {
            if (i <= 6) {
                switch (i) {
                    case 1: cache[i] = 2; break;
                    case 2: cache[i] = 3; break;
                    case 3: cache[i] = 4; break;
                    case 4: cache[i] = 7; break;
                    case 5: cache[i] = 11; break;
                    case 6: cache[i] = 18; break;
                }
            } else {
                cache[i] = cache[i-1] + ((seq_t)2)*cache[i-3] - cache[i-6];
            }
        }
        stored_max = n;
    }
    return cache[n];
#endif
}
