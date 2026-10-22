/*
 * A320497  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent hexagons with respect to a point of type C.
 * https://oeis.org/A320497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A320497 – Coordination sequence of the thinnest 5‑neighbor
 * packing of the plane with congruent hexagons with respect to a type C point.
 *
 * Recurrence (for n > 25):
 *   a(n) = a(n-6) + a(n-16) - a(n-22)
 * Initial terms (n = 0..25):
 *   1, 4, 8, 10, 14, 16, 22, 28, 32, 36, 36, 42, 48,
 *   50, 54, 58, 60, 68, 74, 72, 76, 80, 86, 94, 96, 94
 */

#include <stdlib.h>

/* Initial values up to n = 25   (stored as 64‑bit unsigned constants) */
static const unsigned long long init_vals[26] = {
    1ULL, 4ULL, 8ULL, 10ULL, 14ULL, 16ULL, 22ULL, 28ULL, 32ULL, 36ULL,
    36ULL, 42ULL, 48ULL, 50ULL, 54ULL, 58ULL, 60ULL, 68ULL, 74ULL,
    72ULL, 76ULL, 80ULL, 86ULL, 94ULL, 96ULL, 94ULL
};

/* Cache for computed terms with n >= 26 */
static seq_t *cache = NULL;          /* pointer to cache array (allocated on demand) */
static size_t cache_len = 0;         /* current allocated length of cache */
static idx_t  cache_max = 25;        /* largest index for which a(n) is stored in cache */

/* Compute a(n) ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Return initial value if n < 26 */
    if (n < 26)
        return (seq_t) init_vals[(size_t)n];

    /* Allocate or resize cache if necessary */
    if (cache == NULL) {
        cache_len = (n >= 26) ? (size_t)(n + 1) : 26;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            abort();
    } else if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t i6  = i - 6;
        idx_t i16 = i - 16;
        idx_t i22 = i - 22;

        seq_t a6  = (i6  < 26) ? (seq_t)init_vals[(size_t)i6]  : cache[(size_t)i6];
        seq_t a16 = (i16 < 26) ? (seq_t)init_vals[(size_t)i16] : cache[(size_t)i16];
        seq_t a22 = (i22 < 26) ? (seq_t)init_vals[(size_t)i22] : cache[(size_t)i22];

        cache[(size_t)i] = a6 + a16 - a22;
    }

    cache_max = n;
    return cache[(size_t)n];
}
