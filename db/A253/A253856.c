/*
 * A253856  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum plus antidiagonal minimum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -13*a(n-2) -8*a(n-3) +77*a(n-4) -392*a(n-5)
 *            +721*a(n-6) -504*a(n-7) +176*a(n-8) +384*a(n-9) -960*a(n-10)
 *            +512*a(n-11) Initial values (a(1) … a(11)): 512, 3264, 19456,
 *            114880, 660480, 3727552, 20872192, 116568256, 649733120,
 *            3617345728, 20128173056
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;   /* number of terms stored (valid for n >= 1) */

    if (n == 0)
        return 0;   /* undefined below the offset */

    size_t target = (size_t)n;

    if (size < target) {
        /* Grow cache if necessary */
        if (cap < target) {
            size_t newcap = cap ? cap << 1U : 16;
            while (newcap < target)
                newcap <<= 1U;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)
                return 0;   /* allocation failure – unlikely in tests */
            cache = tmp;
            cap   = newcap;
        }

        /* Initial terms a(1) … a(11) */
        static const seq_t init[11] = {
            (seq_t)512ULL,
            (seq_t)3264ULL,
            (seq_t)19456ULL,
            (seq_t)114880ULL,
            (seq_t)660480ULL,
            (seq_t)3727552ULL,
            (seq_t)20872192ULL,
            (seq_t)116568256ULL,
            (seq_t)649733120ULL,
            (seq_t)3617345728ULL,
            (seq_t)20128173056ULL
        };

        for (size_t i = size + 1; i <= target; ++i) {
            if (i <= 11) {
                cache[i - 1] = init[i - 1];
            } else {
                seq_t sum = 0;
                sum += (seq_t)8   * cache[i - 2];   /* a(n-1) */
                sum -= (seq_t)13  * cache[i - 3];   /* a(n-2) */
                sum -= (seq_t)8   * cache[i - 4];   /* a(n-3) */
                sum += (seq_t)77  * cache[i - 5];   /* a(n-4) */
                sum -= (seq_t)392 * cache[i - 6];   /* a(n-5) */
                sum += (seq_t)721 * cache[i - 7];   /* a(n-6) */
                sum -= (seq_t)504 * cache[i - 8];   /* a(n-7) */
                sum += (seq_t)176 * cache[i - 9];   /* a(n-8) */
                sum += (seq_t)384 * cache[i -10];   /* a(n-9) */
                sum -= (seq_t)960 * cache[i -11];   /* a(n-10) */
                sum += (seq_t)512 * cache[i -12];   /* a(n-11) */
                cache[i - 1] = sum;
            }
        }

        size = target;
    }

    return cache[n - 1];
}
