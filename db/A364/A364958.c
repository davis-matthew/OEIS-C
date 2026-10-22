/*
 * A364958  Fixed points of A356867, where A356867 is Sycamore's Doudna
 *          variant D(3).
 * https://oeis.org/A364958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest integer of the form k * 3^m, where k ∈
 *            {1, 2, 8, 91} (sorted in increasing order). This is equivalent
 *            to a(n) = fixed points of A356867
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   grow_seq, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow3.h"
#include "../utils/grow_seq.h"

/* External types defined by the build */
typedef uint64_t idx_t;   /* placeholder – real definition provided by the build */

seq_t a(idx_t n)
{
    /* Bases that generate the sequence: 1·3^m, 2·3^m, 8·3^m, 91·3^m   */
    static const seq_t base[4] = { (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)91 };

    /* Current exponent for each base */
    static idx_t exp[4]   = {0, 0, 0, 0};
    /* Current candidate value for each base */
    static seq_t val[4]   = {0, 0, 0, 0};

    /* Cache of generated sequence terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;    /* number of terms already generated */
    static size_t cache_cap = 0;    /* allocated capacity of the cache */

    /* One‑time initialization of candidate values */
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 4; ++i)
            val[i] = base[i];     /* base[i] * 3^0 */
        cache      = NULL;
        cache_len  = 0;
        cache_cap  = 0;
        initialized = 1;
    }

    /* Ensure we have computed at least n terms */
    while (cache_len < n) {
        /* Find the smallest current candidate */
        seq_t minimum = val[0];
        int min_idx  = 0;
        for (int i = 1; i < 4; ++i) {
            if (val[i] < minimum) {
                minimum = val[i];
                min_idx = i;
            }
        }

        /* Append the minimum to the cache */
        grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = minimum;

        /* Increment the exponent for every base that produced the minimum */
        for (int i = 0; i < 4; ++i) {
            if (val[i] == minimum) {
                ++exp[i];
                val[i] = base[i] * pow3(exp[i]); /* recompute candidate */
            }
        }
    }

    return cache[n - 1];
}
