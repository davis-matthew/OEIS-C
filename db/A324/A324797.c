/*
 * A324797  Irregular triangle read by rows: row n gives denominators of
 *          fractions in the Farey subsequence B(m).
 * https://oeis.org/A324797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, tabf
 * Formula:   a(n) = denominator of the n-th fraction in the irregular
 *            triangle * of OEIS A324797. Each row n lists the denominators of
 *            all reduced * fractions h/k occurring in the Farey subsequence
 *            B(n); the sequence a(n) * is obtained by reading these rows
 *            consecutively (offset 1)
 * Range:     u8 n <= 113, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local GCD on 64‑bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* pair used for sorting fractions */
typedef struct { uint64_t h, k; } fraction_t;

/* comparison function for qsort: compare h1/k1 < h2/k2 */
static int cmp_fraction(const void *a, const void *b)
{
    const fraction_t *p1 = (const fraction_t *)a;
    const fraction_t *p2 = (const fraction_t *)b;
    uint64_t left  = p1->h * p2->k;
    uint64_t right = p2->h * p1->k;
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

/* Compute the n-th term of A324797 (offset 1).  idx_t and seq_t are
 * defined by the build system. */
seq_t a(idx_t n)
{
    /* The sequence index is 1‑based.  We use a running position counter. */
    idx_t target = n;
    idx_t pos = 0;

    for (idx_t row = 1; ; ++row) {
        /* Estimate needed capacity; we grow dynamically. */
        size_t cap = 0, len = 0;
        fraction_t *fracs = NULL;

        /* Generate all fractions in B(row).  Denominator bounds: 1..2*row. */
        for (idx_t k = 1; k <= 2 * row; ++k) {
            if (k == 1) {                  /* 0/1 and 1/1 are special */
                if (len == cap) {
                    cap = cap ? cap * 2 : 4;
                    fracs = (fraction_t *)realloc(fracs, cap * sizeof(fraction_t));
                }
                fracs[len].h = 0; fracs[len].k = 1; ++len;
                if (len == cap) {
                    cap = cap ? cap * 2 : 4;
                    fracs = (fraction_t *)realloc(fracs, cap * sizeof(fraction_t));
                }
                fracs[len].h = 1; fracs[len].k = 1; ++len;
                continue;
            }

            /* Determine allowed numerators h for this denominator k. */
            idx_t low  = (k > row) ? k - row : 1;
            idx_t high = (k < row) ? (uint64_t)(k - 1) : row;  /* h < k */
            if (low > high) continue;

            for (idx_t h = low; h <= high; ++h) {
                if (gcd_u64((uint64_t)h, (uint64_t)k) != 1) continue;
                if (len == cap) {
                    cap = cap ? cap * 2 : 4;
                    fracs = (fraction_t *)realloc(fracs, cap * sizeof(fraction_t));
                }
                fracs[len].h = (uint64_t)h;
                fracs[len].k = (uint64_t)k;
                ++len;
            }
        }

        /* Sort fractions by numeric value. */
        if (len > 1) qsort(fracs, len, sizeof(fraction_t), cmp_fraction);

        /* Walk through sorted fractions, accumulating positions. */
        for (size_t i = 0; i < len; ++i) {
            ++pos;
            if (pos == target) {
                seq_t result = (seq_t)fracs[i].k; /* denominator */
                free(fracs);
                return result;
            }
        }
        free(fracs);
        /* Continue to next row until the target is found. */
    }
}
