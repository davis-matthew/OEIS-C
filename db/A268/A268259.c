/*
 * A268259  Number of length-(n+1) 0..6 arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) -777*a(n-2) +7545*a(n-3) -41215*a(n-4)
 *            +115171*a(n-5) -81663*a(n-6) -274113*a(n-7) +244616*a(n-8)
 *            +557626*a(n-9) +307440*a(n-10) +67488*a(n-11) +5040*a(n-12)
 *            Initial values: a(1)=43, a(2)=265, a(3)=1634, a(4)=10082,
 *            a(5)=62249, a(6)=384605, a(7)=2377935, a(8)=14712729,
 *            a(9)=91096...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[12] = {
    43ULL,
    265ULL,
    1634ULL,
    10082ULL,
    62249ULL,
    384605ULL,
    2377935ULL,
    14712729ULL,
    91096234ULL,
    564452368ULL,
    3500093507ULL,
    21720172553ULL
};

static const int coeffs[12] = {
    43, -777, 7545, -41215, 115171, -81663,
    -274113, 244616, 557626, 307440, 67488, 5040
};

static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_n = 0;        /* number of terms currently stored */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* n is always >=1, safe guard */
    if (n <= 12) return init[n-1];

    /* Ensure cache exists */
    if (cache == NULL) {
        size_t cap = (size_t)n > 12 ? (size_t)n : 12;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failure */
        memcpy(cache, init, sizeof(init));
        cache_n = 12;
    } else if (n > cache_n) {
        /* increase capacity if needed */
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_n; i < n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= 12; ++k) {
            seq_t prev = cache[(size_t)(i - k)];
            int c = coeffs[k - 1];
            if (c >= 0)
                pos += (seq_t)c * prev;
            else
                neg += (seq_t)(-c) * prev;
        }
        cache[(size_t)i] = pos - neg;
    }
    cache_n = n;
    return cache[(size_t)(n - 1)];
}
