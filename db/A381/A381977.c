/*
 * A381977  Number of edge intersections in the divisibility circle graph of n
 *          (base 10).
 * https://oeis.org/A381977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of chords (k,10k mod n) that intersect
 *            when the integers 0..n-1 are evenly spaced on a circle
 * Range:     u8 n <= 42, u16 n <= 616, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   fenwick_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381977 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fenwick_sum.h"

/* Structure to hold a chord's sorted endpoints */
typedef struct {
    idx_t a;  /* left (smaller) endpoint */
    idx_t b;  /* right (larger) endpoint  */
} chord_t;

/* Comparison for qsort: first by left, then by right */
static int cmp_chord(const void *p1, const void *p2)
{
    const chord_t *c1 = (const chord_t *)p1;
    const chord_t *c2 = (const chord_t *)p2;
    if (c1->a < c2->a) return -1;
    if (c1->a > c2->a) return 1;
    if (c1->b < c2->b) return -1;
    if (c1->b > c2->b) return 1;
    return 0;
}

/* Fenwick tree helpers (1‑based indexing) */
static void fenwick_add(uint32_t *bit, size_t size, size_t idx, uint32_t val)
{
    for (; idx < size; idx += idx & -idx) bit[idx] += val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;  /* no chords for n=0 or n=1 */

    /* Allocate temporary array for chords (max n entries) */
    chord_t *all = (chord_t *)malloc((size_t)n * sizeof(chord_t));
    if (!all) return 0;  /* allocation failed */

    size_t count = 0;
    for (idx_t k = 0; k < n; ++k) {
        idx_t m = (idx_t)(((__int128)k * 10) % n);  /* 10*k mod n */
        if (m == k) continue;                      /* self‑loop, ignore */
        idx_t left  = (k < m) ? k : m;
        idx_t right = (k < m) ? m : k;
        all[count].a = left;
        all[count].b = right;
        ++count;
    }
    if (count == 0) { free(all); return 0; }

    /* Sort chords by left then right */
    qsort(all, count, sizeof(chord_t), cmp_chord);

    /* Deduplicate identical chords (same a and b) */
    size_t uniq = 0;
    chord_t *chin = all;  /* re‑use buffer for unique chords */
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 || all[i].a != all[i-1].a || all[i].b != all[i-1].b) {
            chin[uniq++] = all[i];
        }
    }

    /* Prepare Fenwick tree: indices 1 .. n (right+1 maps to idx) */
    size_t bit_size = (size_t)n + 2;
    uint32_t *bit = (uint32_t *)calloc(bit_size, sizeof(uint32_t));
    if (!bit) { free(all); return 0; }

    seq_t total = 0;
    size_t idx = 0;
    while (idx < uniq) {
        size_t start = idx;
        idx_t current_left = chin[idx].a;
        /* Find group of chords with the same left endpoint */
        while (idx < uniq && chin[idx].a == current_left) ++idx;
        size_t end = idx; /* exclusive */

        /* First count intersections for this group against earlier chords */
        for (size_t j = start; j < end; ++j) {
            idx_t l = chin[j].a;
            idx_t r = chin[j].b;
            /* count rights in (l, r) among earlier chords */
            uint32_t cnt = fenwick_sum(bit, bit_size, (size_t)r)
                         - fenwick_sum(bit, bit_size, (size_t)l + 1);
            total += (seq_t)cnt;
        }

        /* Then add all rights of this group to the Fenwick tree */
        for (size_t j = start; j < end; ++j) {
            fenwick_add(bit, bit_size, (size_t)chin[j].b + 1, 1);
        }
    }

    free(bit);
    free(all);
    return total;
}
