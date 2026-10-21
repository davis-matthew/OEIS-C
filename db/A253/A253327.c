/*
 * A253327  Number of (2+1)X(n+1) 0..1 arrays with every 2X2 subblock sum
 *          nondecreasing horizontally, vertically and antidiagonally
 *          ne-to-sw.
 * https://oeis.org/A253327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) - 50*a(n-3) - 9*a(n-4) + 270*a(n-5) *
 *            - 128*a(n-6) - 820*a(n-7) + 709*a(n-8) + 1520*a(n-9) * -
 *            1824*a(n-10) - 1722*a(n-11) + 2797*a(n-12) + 1070*a(n-13) * -
 *            2696*a(n-14) - 160*a(n-15) + 1608*a(n-16) * - 240*a(n-17) -
 *            544*a(n-18) + 160*a(n-19) + 80*a(n-20) * - 32*a(n-21)...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system; do not #define them. */

static seq_t *cache = NULL;          /* 1‑based cache; index 0 unused */
static size_t cache_cap = 0;         /* currently allocated capacity */
static size_t computed_up_to = 0;    /* largest n for which cache[n] is valid */
static bool initialized = false;

/* Ensure the cache has room for indices up to 'need' (1‑based). */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first 21 terms. */
static void initialize(idx_t /*n*/)
{
    static const seq_t init[21] = {
        44, 126, 373, 1026, 2797, 6936, 17197,
        39616, 91571, 200800, 442031, 937998,
        1997242, 4147312, 8635487, 17680582, 36274902,
        73603454, 149571217, 301772250, 609502075
    };

    /* We need space for indices 1..21. */
    ensure_capacity(22);
    for (idx_t i = 1; i <= 21; ++i) {
        cache[(size_t)i] = init[(size_t)(i-1)];
    }
    computed_up_to = 21;
    initialized = true;
}

/* Coefficients of the recurrence (a(n-1), a(n-2), …, a(n-21)). */
static const int coeffs[21] = {
     4,   8, -50,  -9,  270, -128, -820, 709, 1520,
   -1824,-1722, 2797, 1070, -2696, -160, 1608,
    -240, -544,  160,  80, -32
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined; return 0 */

    if (!initialized) initialize(n);

    /* Ensure cache large enough. */
    size_t need = (size_t)n + 1;   /* keep index 0 unused */
    ensure_capacity(need);

    /* Return if already known. */
    if ((size_t)n <= computed_up_to) return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (size_t i = computed_up_to + 1; i <= (size_t)n; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (size_t k = 0; k < 21; ++k) {
            int c = coeffs[k];
            if (c == 0) continue;
            seq_t term = cache[i - k - 1];
            if (c > 0)
                pos_sum += term * (seq_t)c;
            else
                neg_sum += term * (seq_t)(-c);
        }
        cache[i] = pos_sum - neg_sum;
    }
    computed_up_to = (size_t)n;
    return cache[(size_t)n];
}
