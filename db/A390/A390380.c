/*
 * A390380  Integers k which cannot be written in the form x^2 + y^2 - z^2,
 *          where x, y, z are integers and x^2, y^2, z^2 <= k.
 * https://oeis.org/A390380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390380: integers k which cannot be written in the form
   x^2 + y^2 - z^2 with integers x, y, z and x^2, y^2, z^2 <= k.
   The routine pre‑computes all such k up to a fixed limit
   (chosen high enough to cover the whole sequence, which
   is conjectured to end at 6563) and returns the n‑th term. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Choose a limit that safely exceeds the largest term
   (6563) and is large enough to collect all positions. */
#define LIMIT 7000

/* Number of non‑representable values found. */
static size_t non_rep_count = 0;
/* Array holding the non‑representable values in increasing order. */
static seq_t *non_rep = NULL;

/* One‑time initialization flag and storage for flags. */
static bool initialized = false;

/* Integer square root for 32/64‑bit values. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Perform the pre‑computation once. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Flag array: 1 if representable, 0 otherwise. */
    unsigned char *rep = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!rep) exit(1);   /* fatal, but build will never hit this */

    /* Pre‑compute all representable values. */
    uint64_t maxroot = isqrt_u64(LIMIT);
    for (uint64_t x = 0; x <= maxroot; ++x) {
        uint64_t sqx = x * x;
        for (uint64_t y = 0; y <= maxroot; ++y) {
            uint64_t sqy = y * y;
            uint64_t s = sqx + sqy;           /* x^2 + y^2 */
            for (uint64_t z = 0; z <= maxroot; ++z) {
                uint64_t sqz = z * z;
                if (sqz > s) break;           /* s - z^2 negative */
                uint64_t k = s - sqz;
                if (k == 0 || k > LIMIT) continue;
                /* Need all squares <= k */
                if (sqx > k || sqy > k || sqz > k) continue;
                rep[(size_t)k] = 1;
            }
        }
    }

    /* Count non‑representable numbers and store them. */
    for (size_t k = 1; k <= (size_t)LIMIT; ++k)
        if (!rep[k]) ++non_rep_count;

    non_rep = (seq_t *)malloc(non_rep_count * sizeof(seq_t));
    if (!non_rep) exit(1);

    size_t idx = 0;
    for (size_t k = 1; k <= (size_t)LIMIT; ++k)
        if (!rep[k]) non_rep[idx++] = (seq_t)k;

    free(rep);
}

/* The sequence function.  Returns 0 if n is out of range. */
seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0) return (seq_t)0;
    if (n > non_rep_count) return (seq_t)0;
    return non_rep[n - 1];
}
