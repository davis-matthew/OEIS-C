/*
 * A249320  Number of length 1+6 0..n arrays with no seven consecutive terms
 *          having six times any element equal to the sum of the remaining
 *          six.
 * https://oeis.org/A249320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -16*a(n-2) +26*a(n-3) -30*a(n-4) +27*a(n-5) *
 *            -21*a(n-6) +16*a(n-7) -11*a(n-8) +4*a(n-9) +4*a(n-10) *
 *            -11*a(n-11) +16*a(n-12) -21*a(n-13) +27*a(n-14) -30*a(n-15) *
 *            +26*a(n-16) -16*a(n-17) +6*a(n-18) - a(n-19) * Initial values:
 *            a(1)=126, a(2)=1792, a(3)=14336, a(4)=68712, a(5)=2490...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_max = 0;
static bool cache_initialized = false;

/* Initial values for n = 1 .. 19 (index 0 unused) */
static const seq_t init_vals[20] = {
    0ULL,
    126ULL, 1792ULL, 14336ULL, 68712ULL, 249088ULL,
    739284ULL, 1898582ULL, 4361056ULL, 9176664ULL,
    17982930ULL, 33230442ULL, 58450770ULL, 98581896ULL,
    160353634ULL, 252737912ULL, 387464994ULL, 579621658ULL,
    848320494ULL, 1217462624ULL
};

/* Coefficients for the 19‑term recurrence (from a(n-1) down to a(n-19)) */
static const int coeff[19] = {
    6, -16, 26, -30, 27, -21, 16, -11, 4, 4,
    -11, 16, -21, 27, -30, 26, -16, 6, -1
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Lazy initialization with the first 19 values */
    if (!cache_initialized) {
        cache = (seq_t *)malloc((19 + 1) * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */
        for (idx_t i = 1; i <= 19; ++i) cache[i] = init_vals[i];
        cache[0] = 0;     /* unused */
        cache_max = 19;
        cache_initialized = true;
    }

    if (n <= cache_max) return cache[n];

    /* Extend the cache to hold up to index n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;   /* allocation failure */
    cache = tmp;

    /* Compute values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
#if !defined(__cplusplus) && defined(__SIZEOF_INT128__)
        /* C11 branch – use 128‑bit integer for exact intermediate arithmetic */
        __int128 acc = 0;
        for (int k = 0; k < 19; ++k) {
            acc += (__int128)coeff[k] * (__int128)cache[i - 1 - (size_t)k];
        }
        cache[i] = (seq_t)acc;
#else
        /* C++ or when seq_t is an arbitrary‑precision type */
        seq_t acc = 0;
        for (int k = 0; k < 19; ++k) {
            idx_t idx = i - 1 - (size_t)k;
            if (coeff[k] >= 0) {
                acc += cache[idx] * (seq_t)coeff[k];
            } else {
                acc -= cache[idx] * (seq_t)(-coeff[k]);
            }
        }
        cache[i] = acc;
#endif
    }

    cache_max = n;
    return cache[n];
}
