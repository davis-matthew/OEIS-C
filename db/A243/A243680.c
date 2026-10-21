/*
 * A243680  Nonnegative integers of the form 2x^2+xy+5y^2.
 * https://oeis.org/A243680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth non‑negative integer representable by * 2x² + x
 *            y + 5y² (x, y ∈ ℤ). The sequence is sorted and * duplicates are
 *            removed. a(0) = 0
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Compare two idx_t values for qsort. */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *ia = (const idx_t *)a;
    const idx_t *ib = (const idx_t *)b;
    if (*ia < *ib) return -1;
    if (*ia > *ib) return 1;
    return 0;
}

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0) return (seq_t)0;

    idx_t bound = (n + 1) * 10ULL + 10ULL;            /* initial bound   */
    for (;;) {
        /* ----- Enumerate all pairs (x,y) with value ≤ bound ----- */
        idx_t maxY = (idx_t)floor(sqrt((double)bound / 5.0));
        idx_t maxX = (idx_t)floor(sqrt((double)bound / 2.0)) + 5;

        size_t estimated = (size_t)(maxY + 1) * (size_t)(2 * maxX + 1) + 1;
        idx_t *buf = (idx_t *)malloc(estimated * sizeof(idx_t));
        if (!buf) return (seq_t)0;                /* allocation failed – return 0 */

        size_t sz = 0;
        for (idx_t y = 0; y <= maxY; ++y) {
            for (idx_t x = 0; x <= maxX; ++x) {
                /* consider both signs of x */
                for (int s = -1; s <= 1; s += 2) {
                    __int128 ix = (__int128)s * (__int128)x;
                    __int128 vy = (__int128)y;
                    __int128 val = 2 * ix * ix + ix * vy + 5 * vy * vy;
                    if (val <= bound) {
                        buf[sz++] = (idx_t)val;
                    }
                }
            }
        }

        /* ----- Sort and deduplicate ----- */
        qsort(buf, sz, sizeof(idx_t), cmp_idx);

        size_t uniq = 0;
        for (size_t i = 0; i < sz; ++i) {
            if (i == 0 || buf[i] != buf[i - 1]) {
                buf[uniq++] = buf[i];
            }
        }

        /* ----- Check if we have enough terms ----- */
        if (uniq > n) {
            seq_t result = (seq_t)buf[n];
            free(buf);
            return result;
        }

        /* ----- Not enough terms: increase bound and repeat ----- */
        free(buf);
        bound = bound * 2ULL + 10ULL;
    }
}
