/*
 * A290314  Fifth diagonal sequence of the Sheffer triangle A094816 (special
 *          Charlier).
 * https://oeis.org/A290314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9), n > 8
 *            * Initial terms: a(0)=1, a(1)=89, a(2)=814, a(3)=4179,
 *            a(4)=15659, * a(5)=47775, a(6)=125853, a(7)=296703, a(8)=641058
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 30, u64 n <= 535, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial nine terms.  The literals are ordinary integer constants;
       they will be converted to seq_t, truncating if the value cannot
       fit in the target type – which is acceptable by the specification. */
    static const seq_t init[9] = {
        1ULL, 89ULL, 814ULL, 4179ULL, 15659ULL,
        47775ULL, 125853ULL, 296703ULL, 641058ULL
    };

    /* Cache for computed terms: */
    static seq_t *cache = NULL;
    static size_t capacity = 0; /* allocated size */
    static size_t length   = 0; /* number of terms currently stored */

    /* Ensure the cache is initialised with the first nine terms. */
    if (cache == NULL) {
        capacity = 9;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 9; ++i) cache[i] = init[i];
        length = 9;
    }

    size_t idx = (size_t)n;

    /* Basic need check for small indices. */
    if (idx < 9) return cache[idx];

    /* Ensure we have enough room for idx. */
    if (capacity <= idx) {
        size_t new_cap = capacity ? capacity : 9;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        capacity = new_cap;
    }

    /* Recurrence coefficients. */
    static const int coeff[9] = { 9, -36, 84, -126, 126, -84, 36, -9, 1 };

    /* Compute missing terms up to n. */
    for (size_t i = length; i <= idx; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 9; ++j) {
            seq_t t = cache[i - 1 - j];
            if (coeff[j] >= 0)
                sum += t * (seq_t)coeff[j];
            else
                sum -= t * (seq_t)(-coeff[j]);
        }
        cache[i] = sum;
    }
    length = idx + 1;

    return cache[idx];
}
