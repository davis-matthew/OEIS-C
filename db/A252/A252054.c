/*
 * A252054  Number of perfect matchings in the P_4 X C_n graph.
 * https://oeis.org/A252054
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product(13-14*cos(2*(2*j-1)*Pi/n)+2*cos(4*(2*j-1)*Pi/n),
 *            j=1..floor(n/2)). Equivalent linear recurrence (order 14): a(n)
 *            = a(n-1)+13*a(n-2)-7*a(n-3)-61*a(n-4)+12*a(n-5)+128*a(n-6)
 *            -128*a(n-8)-12*a(n-9)+61*a(n-10)+7*a(n-11)-13*a(n-12)-a(n-13)+a(n-14),
 *            valid for n ≥ 17. Initial values (n = 3...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#  include <vector>
#else
#  include <stdlib.h>
#endif
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 3)
        return 0;      /* undefined for n < 3 */

    static const seq_t base_vals[14] = {
        19, 121, 176, 725, 1471, 5041, 11989, 37584,
        97021, 290521, 783511, 2289869, 6323504, 18241441};

#ifdef __cplusplus
    static std::vector<seq_t> cache(3);   /* indices 0,1,2 unused */
    static size_t computed = 3;           /* next index to compute */

    if (n + 1 > cache.size())
        cache.resize(n + 1);

    while (computed <= (size_t)n) {
        size_t m = computed;
        if (m <= 16)
            cache[m] = base_vals[m - 3];
        else
            cache[m] = cache[m - 1]
                       + (seq_t)13 * cache[m - 2]
                       - (seq_t)7  * cache[m - 3]
                       - (seq_t)61 * cache[m - 4]
                       + (seq_t)12 * cache[m - 5]
                       + (seq_t)128 * cache[m - 6]
                       - (seq_t)128 * cache[m - 8]
                       - (seq_t)12  * cache[m - 9]
                       + (seq_t)61  * cache[m -10]
                       + (seq_t)7   * cache[m -11]
                       - (seq_t)13  * cache[m -12]
                       - cache[m - 13]
                       + cache[m - 14];
        ++computed;
    }
    return cache[n];
#else
    /* C implementation – seq_t is a plain unsigned integer type */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t  next   = 0;        /* next index to compute */

    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 32;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    if (next == 0) {                /* initialise 0,1,2 to zero */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        next    = 3;
    }

    for (idx_t m = next; m <= n; ++m) {
        if (m <= 16)
            cache[m] = base_vals[m - 3];
        else {
            __int128 sum = 0;
            sum += (__int128)cache[m - 1];
            sum += (__int128)13  * cache[m - 2];
            sum -= (__int128)7   * cache[m - 3];
            sum -= (__int128)61  * cache[m - 4];
            sum += (__int128)12  * cache[m - 5];
            sum += (__int128)128 * cache[m - 6];
            sum -= (__int128)128 * cache[m - 8];
            sum -= (__int128)12  * cache[m - 9];
            sum += (__int128)61  * cache[m -10];
            sum += (__int128)7   * cache[m -11];
            sum -= (__int128)13  * cache[m -12];
            sum -= (__int128)cache[m - 13];
            sum += (__int128)cache[m - 14];
            cache[m] = (seq_t)sum;
        }
    }
    next = n + 1;
    return cache[n];
#endif
}
