/*
 * A251651  Number of (n+2)X(7+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+3*a(n-2)+101*a(n-3)-438*a(n-4)-329*a(n-5)+990*a(n-6)-220*a(n-7)-110*a(n-8),
 *            for n > 13. Initial values (for n = 1..13): a(1) = 6978 a(2) =
 *            49956 a(3) = 163318 a(4) = 610082 a(5) = 5248890 a(6) = 17185778
 *            a(7) = 63447690 a(8) = 562400186 a(9) = 1826236602 a(10) =
 *            6707952986 a(11) =...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 23, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t cache[20001];          /* Holds values up to n = 20000 (safe for n ≤ 10000) */
static idx_t cache_size = 0;        /* Number of initialised entries in cache */

static const seq_t init_vals[13] = {
    (seq_t)6978ULL,
    (seq_t)49956ULL,
    (seq_t)163318ULL,
    (seq_t)610082ULL,
    (seq_t)5248890ULL,
    (seq_t)17185778ULL,
    (seq_t)63447690ULL,
    (seq_t)562400186ULL,
    (seq_t)1826236602ULL,
    (seq_t)6707952986ULL,
    (seq_t)60713749434ULL,
    (seq_t)196015117610ULL,
    (seq_t)717238558554ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_size = 13;
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
    }

    if (n <= cache_size)
        return cache[n-1];

    /* compute remaining terms up to n using the linear recurrence */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t a1 = cache[i-1];   /* a(i)   */
        seq_t a2 = cache[i-2];   /* a(i-1) */
        seq_t a3 = cache[i-3];   /* a(i-2) */
        seq_t a4 = cache[i-4];   /* a(i-3) */
        seq_t a5 = cache[i-5];   /* a(i-4) */
        seq_t a6 = cache[i-6];   /* a(i-5) */
        seq_t a7 = cache[i-7];   /* a(i-6) */
        seq_t a8 = cache[i-8];   /* a(i-7) */

        seq_t pos = ((seq_t)4) * a1;
        pos += ((seq_t)3) * a2;
        pos += ((seq_t)101) * a3;
        pos += ((seq_t)990) * a6;

        seq_t neg = ((seq_t)438) * a4;
        neg += ((seq_t)329) * a5;
        neg += ((seq_t)220) * a7;
        neg += ((seq_t)110) * a8;

        cache[i] = pos - neg;   /* recurrence result */
    }

    cache_size = n;
    return cache[n-1];
}
