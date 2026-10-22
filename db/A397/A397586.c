/*
 * A397586  Number of ways to tile a strip of length n with tiles of length
 *          equivalent to 1,3,4, or 5 (mod 6).
 * https://oeis.org/A397586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-6), a(0) = 1,
 *            a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 7, a(6) = 11
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use a static std::vector for dynamic storage (safe for arbitrary‑precision types) */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        /* initialise a(0) */
        cache.push_back((seq_t)1); /* a(0) */
    }

    if ((idx_t)cache.size() <= n) {
        /* compute missing values up to n */
        for (idx_t i = (idx_t)cache.size(); i <= n; ++i) {
            seq_t val;
            if (i <= 6) {
                switch ((int)i) {
                    case 1: case 2: val = (seq_t)1; break;
                    case 3: val = (seq_t)2; break;
                    case 4: val = (seq_t)4; break;
                    case 5: val = (seq_t)7; break;
                    case 6: val = (seq_t)11; break;
                }
            } else {
                val = cache[i-1] + cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
            }
            cache.push_back(val);
        }
    }

    return cache[n];
#else
    /* C: use malloc/realloc to manage an array of seq_t (POD types only) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t used  = 0;   /* number of values computed (cache[0..used-1]) */

    /* Ensure capacity for indices up to n */
    if (n + 1 > cap) {
        idx_t new_cap = (cap == 0) ? 8 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        /* realloc is safe for seq_t POD types; cast to seq_t* */
        seq_t *tmp = realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = used; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i <= 6) {
            switch ((int)i) {
                case 1: case 2: val = (seq_t)1; break;
                case 3: val = (seq_t)2; break;
                case 4: val = (seq_t)4; break;
                case 5: val = (seq_t)7; break;
                case 6: val = (seq_t)11; break;
                default: val = 0; /* unreachable */
            }
        } else {
            val = cache[i-1] + cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
        }
        cache[i] = val;
    }

    used = n + 1;
    return cache[n];
#endif
}
