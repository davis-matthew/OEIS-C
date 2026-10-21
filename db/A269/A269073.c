/*
 * A269073  Number of nX6 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) -230*a(n-2) +192*a(n-3) +3805*a(n-4)
 *            -5776*a(n-5) -27808*a(n-6) +25744*a(n-7) +101333*a(n-8)
 *            -13916*a(n-9) -149690*a(n-10) -66848*a(n-11) +23183*a(n-12)
 *            +9888*a(n-13) -2304*a(n-14) Initial terms: a(1)=41, a(2)=489,
 *            a(3)=7477, a(4)=102545, a(5)=1383105, a(6)=18220241,
 *            a(7)=2362726...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms.  cache[0] = a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Recurrence coefficients (index 0 corresponds to a(n-1)) */
    static const int coeff[14] = {
        28,    -230,   192,    3805,   -5776,
        -27808, 25744, 101333, -13916, -149690,
        -66848, 23183,  9888,   -2304
    };

    /* Pre‑computed first 14 terms.  init[0] = a(1). */
    static const seq_t init[14] = {
        41ULL,
        489ULL,
        7477ULL,
        102545ULL,
        1383105ULL,
        18220241ULL,
        236272677ULL,
        3024972401ULL,
        38333973609ULL,
        481701017577ULL,
        6010309951205ULL,
        74542025956769ULL,
        919716929870465ULL,
        11296618314880209ULL
    };

    if (n == 0) return 0;            /* a(0) is unused */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 14;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* Extend cache to include up to a(n) */
    if (n > cache_len) {
        /* Reallocate to hold n terms */
        seq_t *newptr = (seq_t *)realloc(cache,
                                         (size_t)n * sizeof(seq_t));
        if (newptr == NULL) {
            /* Out of memory; return 0 as a safe failure value. */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return 0;
        }
        cache = newptr;

        /* Compute missing terms */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t pos = 0, neg = 0;
            /* a(n) depends on the previous 14 terms */
            for (int k = 1; k <= 14; ++k) {
                seq_t term = cache[i - k];
                seq_t mul;
                /* multiply term by |coeff[k-1]| */
                mul = term * (seq_t)(coeff[k - 1] >= 0 ?
                                     coeff[k - 1] : -coeff[k - 1]);
                if (coeff[k - 1] >= 0)
                    pos += mul;
                else
                    neg += mul;
            }
            /* pos ≥ neg for this sequence */
            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
