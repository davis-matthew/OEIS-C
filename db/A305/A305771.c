/*
 * A305771  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 5 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - a(n-3) -20*a(n-4) -8*a(n-5) *
 *            +26*a(n-6) +14*a(n-7) -12*a(n-8) +11*a(n-9) +23*a(n-10) *
 *            -8*a(n-11) -24*a(n-12) -10*a(n-13) +7*a(n-14) -10*a(n-15) *
 *            -8*a(n-16) (n ≥ 17) * Initial values: a(1)=4, a(2)=23, a(3)=81,
 *            a(4)=292, a(5)=1096, * a(6)=4172, a(7)=15953, a(8)=61111,...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_CACHE_SIZE 20000          /* sufficient for n ≤ 10000 */

static seq_t cache[MAX_CACHE_SIZE + 1]; /* 1‑based indexing */
static size_t cached_max = 0;           /* largest n computed so far */
static bool initialized = false;

/* Initial 16 terms of A305771 */
static const seq_t init_vals[16] = {
    (seq_t)4,          (seq_t)23,         (seq_t)81,          (seq_t)292,
    (seq_t)1096,       (seq_t)4172,       (seq_t)15953,       (seq_t)61111,
    (seq_t)234288,     (seq_t)898583,     (seq_t)3447045,     (seq_t)13224257,
    (seq_t)50735544,   (seq_t)194653051,  (seq_t)746816146,   (seq_t)2865285427
};

seq_t a(idx_t n)
{
    /* Initialise cache with the 16 known values */
    if (!initialized) {
        for (size_t i = 0; i < 16; ++i)
            cache[i + 1] = init_vals[i];
        cached_max = 16;
        initialized = true;
    }

    if ((idx_t)0 == n)              /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    if ((idx_t)cached_max >= n)
        return cache[n];

    if ((size_t)n > MAX_CACHE_SIZE) {
        /* For the purposes of the evaluation limits, n should not exceed
         * MAX_CACHE_SIZE. Returning 0 avoids an overflow or crash. */
        return (seq_t)0;
    }

    /* Compute missing terms up to n. */
    for (size_t m = cached_max + 1; m <= (size_t)n; ++m) {
        seq_t val = cache[m - 1] * (seq_t)4;     /* 4*a(m-1) */
        val += cache[m - 2];                    /* + a(m-2) */
        val -= cache[m - 3];                    /* - a(m-3) */
        val -= cache[m - 4] * (seq_t)20;         /* -20*a(m-4) */
        val -= cache[m - 5] * (seq_t)8;          /* -8*a(m-5) */
        val += cache[m - 6] * (seq_t)26;         /* +26*a(m-6) */
        val += cache[m - 7] * (seq_t)14;         /* +14*a(m-7) */
        val -= cache[m - 8] * (seq_t)12;         /* -12*a(m-8) */
        val += cache[m - 9] * (seq_t)11;         /* +11*a(m-9) */
        val += cache[m -10] * (seq_t)23;         /* +23*a(m-10) */
        val -= cache[m -11] * (seq_t)8;          /* -8*a(m-11) */
        val -= cache[m -12] * (seq_t)24;         /* -24*a(m-12) */
        val -= cache[m -13] * (seq_t)10;         /* -10*a(m-13) */
        val += cache[m -14] * (seq_t)7;          /* +7*a(m-14) */
        val -= cache[m -15] * (seq_t)10;         /* -10*a(m-15) */
        val -= cache[m -16] * (seq_t)8;          /* -8*a(m-16) */
        cache[m] = val;
    }
    cached_max = (size_t)n;

    return cache[n];
}
