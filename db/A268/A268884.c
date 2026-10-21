/*
 * A268884  Number of nX6 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows a 10‑term linear recurrence for n > 12 a(n) =
 *            42*a(n-1) - 665*a(n-2) + 5138*a(n-3) - 21972*a(n-4) +
 *            55274*a(n-5) - 83769*a(n-6) + 76202*a(n-7) - 40273*a(n-8) +
 *            11304*a(n-9) - 1296*a(n-10) initial values: a(1) … a(12) are
 *            taken from the OEIS b‑file
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t init_vals[12] = {
    (seq_t)20ULL,
    (seq_t)475ULL,
    (seq_t)9996ULL,
    (seq_t)186732ULL,
    (seq_t)3283890ULL,
    (seq_t)55491832ULL,
    (seq_t)911930096ULL,
    (seq_t)14681855846ULL,
    (seq_t)232688402028ULL,
    (seq_t)3642322709900ULL,
    (seq_t)56444213311842ULL,
    (seq_t)867475989937560ULL
};

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cache[10000]; /* enough for all calls up to n < 10000 */
    static size_t cache_size = 0;

    if (!initialized) {
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_size = 12;
        initialized = 1;
    }

    if (n <= cache_size) {
        return cache[n-1];
    }

    /* extend cache up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* i+1 = current term index (1‑based) */
        seq_t t = (seq_t)42 * cache[i-1]
                - (seq_t)665 * cache[i-2]
                + (seq_t)5138 * cache[i-3]
                - (seq_t)21972 * cache[i-4]
                + (seq_t)55274 * cache[i-5]
                - (seq_t)83769 * cache[i-6]
                + (seq_t)76202 * cache[i-7]
                - (seq_t)40273 * cache[i-8]
                + (seq_t)11304 * cache[i-9]
                - (seq_t)1296 * cache[i-10];
        cache[i] = t;
    }
    cache_size = (size_t)n;
    return cache[n-1];
}
