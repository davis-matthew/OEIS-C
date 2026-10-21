/*
 * A248436  Number of length 3+2 0..n arrays with every three consecutive
 *          terms having the sum of some two elements equal to twice the
 *          third.
 * https://oeis.org/A248436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the recurrence a(n) =
 *            a(n-1)-a(n-2)+a(n-3)-a(n-4)+2a(n-5)-a(n-6)+a(n-7)
 *            -a(n-11)+a(n-12)-2a(n-13)+a(n-14)-a(n-15)+a(n-16)-a(n-17)+a(n-18)
 *            with initial values (n from 1): 2, 9, 24, 69, 118, 185, 252,
 *            357, 470, 593, 716, 881, 1046, 1217, 1400, 1621, 1842, 2081. The
 *            array is computed on...
 * Range:     u8 n <= 7, u16 n <= 97, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* The maximum n we expect to handle in the tests ( ~10000 ) */
#define MAX_N 10000

/* seq_t and idx_t are provided by the build system, see the
   requirements section. They are used directly in the code below. */

static seq_t cache[MAX_N + 1];   /* index 0 unused, cache[1]..cache[MAX_N] */
static size_t cache_len = 0;     /* largest n for which cache[n] is valid */

/* Initialise the cache with the 18 known initial values. */
static void init_cache(void)
{
    /* If already initialised, nothing to do. */
    if (cache_len >= 18)
        return;

    static const seq_t init[19] = {
        0ULL,               /* unused 0 index - keep array 1‑based */
        2ULL, 9ULL, 24ULL, 69ULL, 118ULL, 185ULL, 252ULL, 357ULL,
        470ULL, 593ULL, 716ULL, 881ULL, 1046ULL, 1217ULL,
        1400ULL, 1621ULL, 1842ULL, 2081ULL
    };

    for (size_t i = 1; i <= 18; ++i) {
        cache[i] = init[i];
    }
    cache_len = 18;
}

/* Compute a(n).  The function is thread‑safe for single‑threaded
   use as required by the test harness. */
seq_t a(idx_t n)
{
    /* Clamp n to the pre‑allocated buffer to avoid out‑of‑bounds. */
    if (n >= MAX_N + 1) {
        /* For n beyond the allocated buffer we perform the computation
           on a temporary array of size (n+1) using malloc and
           placement‑new to initialise seq_t objects.  This part is
           only used when the symmetry limits are exceeded.  It
           guarantees correctness even for arbitrary big‑int seq_t. */
        size_t needed = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!tmp)
            return 0;            /* allocation failed – return 0 */

        /* Default construct the array elements via placement new if
           seq_t is a class type.  In C this block is a no‑op.        */
#if defined(__cplusplus)
        for (size_t i = 0; i < needed; ++i)
            new (&tmp[i]) seq_t();
#endif

        /* Initialise first 18 elements. */
        static const seq_t init[19] = {
            0ULL, 2ULL, 9ULL, 24ULL, 69ULL, 118ULL, 185ULL, 252ULL, 357ULL,
            470ULL, 593ULL, 716ULL, 881ULL, 1046ULL, 1217ULL,
            1400ULL, 1621ULL, 1842ULL, 2081ULL
        };
        for (size_t i = 1; i <= 18 && i <= n; ++i)
            tmp[i] = init[i];

        /* Compute remaining values up to n. */
        for (size_t i = 19; i <= (size_t)n; ++i) {
            tmp[i] =  tmp[i-1] - tmp[i-2] + tmp[i-3] - tmp[i-4] + 2*tmp[i-5]
                     - tmp[i-6] + tmp[i-7] - tmp[i-11] + tmp[i-12]
                     - 2*tmp[i-13] + tmp[i-14] - tmp[i-15] + tmp[i-16]
                     - tmp[i-17] + tmp[i-18];
        }

        /* Return the requested value. */
        seq_t result = tmp[n];

        /* Clean up the temporary array.  In C++ call destructors. */
#if defined(__cplusplus)
        for (size_t i = 0; i < needed; ++i)
            tmp[i].~seq_t();
#endif
        free(tmp);
        return result;
    }

    /* If we are within the pre‑allocated buffer, ensure cache is ready. */
    init_cache();

    /* Compute values up to n if not already done. */
    if (cache_len < (size_t)n) {
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4] + 2*cache[i-5]
                       - cache[i-6] + cache[i-7] - cache[i-11] + cache[i-12]
                       - 2*cache[i-13] + cache[i-14] - cache[i-15] + cache[i-16]
                       - cache[i-17] + cache[i-18];
        }
        cache_len = (size_t)n;
    }
    return cache[n];
}
