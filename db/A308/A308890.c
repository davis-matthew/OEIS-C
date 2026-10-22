/*
 * A308890  Follow along the squares in the square spiral (as in A274640); in
 *          each square write the smallest positive number that a knight
 *          placed at that square cannot see.
 * https://oeis.org/A308890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{ a(m) : m < n and (x_n,y_n) and (x_m,y_m) differ by a
 *            knight's move }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   xy_to_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   and the spiral is the same as in A274640 (right, up, left, down). */
/*   a(0) is unused; a(1) = 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/xy_to_index.h"

/* Helper: integer square root (floor) of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t n)
{
    if (n <= 1) return n;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Map spiral index n (1‑based) to its (x,y) coordinates. The first square
   (n=1) is at (0,0).  The spiral proceeds right, up, left, down, etc. */
static void index_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n <= 1) { *x = 0; *y = 0; return; }

    idx_t isq = isqrt_u64(n);
    if (isq * isq < n) ++isq;                 /* ceil sqrt */
    if ((isq & 1) == 0) ++isq;                /* odd side length */

    idx_t r = (isq - 1) >> 1;            /* ring number, r >= 1 */
    idx_t side = 2 * r + 1;              /* side length of the ring */
    idx_t mmax = side * side;            /* index of bottom‑right corner */
    idx_t d = mmax - n;                  /* offset from that corner */

    if (d <= 2 * r) {                    /* bottom side, moving left */
        *x = (int64_t)r - (int64_t)d;
        *y = -(int64_t)r;
    } else if (d <= 4 * r) {             /* left side, moving up */
        *x = -(int64_t)r;
        *y = -(int64_t)r + (int64_t)(d - 2 * r);
    } else if (d <= 6 * r) {             /* top side, moving right */
        *x = -(int64_t)r + (int64_t)(d - 4 * r);
        *y = (int64_t)r;
    } else {                             /* right side, moving down */
        *x = (int64_t)r;
        *y = (int64_t)r - (int64_t)(d - 6 * r);
    }
}

/* Cache of already computed a(n).  0 means “not yet computed”, because
   a(n) is always in 1..5.  We store only uint8_t values to avoid
   construction/destruction issues with arbitrary‑precision seq_t. */
static uint8_t *val_cache = NULL;
static size_t cache_cap = 0;

/* Ensure that val_cache can hold at least n entries. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    uint8_t *new_cache = (uint8_t*)realloc(val_cache, (size_t)n * sizeof(uint8_t));
    if (!new_cache) return;          /* allocation failure – leave cache unchanged */
    /* initialise new slots to 0 (sentinel) */
    for (size_t i = cache_cap; i < (size_t)n; ++i) new_cache[i] = 0;
    val_cache = new_cache;
    cache_cap = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    if (val_cache[n - 1] != 0) return (seq_t)val_cache[n - 1];

    if (n == 1) {                      /* centre square */
        val_cache[0] = 1;
        return (seq_t)1;
    }

    int64_t x, y;
    index_to_xy(n, &x, &y);

    /* Knight move offsets */
    static const int dx[8] = { 1, 1, -1, -1,  2,  2, -2, -2};
    static const int dy[8] = { 2,-2,  2, -2,  1, -1,  1, -1};

    bool seen[6] = {false, false, false, false, false, false};

    for (int i = 0; i < 8; ++i) {
        int64_t nx = x + dx[i];
        int64_t ny = y + dy[i];
        idx_t m = xy_to_index(nx, ny);
        if (m >= 1 && m < n) {
            seq_t val = a(m);
            if (val <= 5) seen[(size_t)val] = true;
        }
    }

    seq_t result = 1;
    while (result <= 5 && seen[(size_t)result]) ++result;
    val_cache[n - 1] = (uint8_t)result;
    return result;
}
