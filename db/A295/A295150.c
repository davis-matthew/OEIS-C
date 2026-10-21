/*
 * A295150  Numbers that have exactly two representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation: a(n) returns the n‑th integer that has exactly two
 * representations as a sum of five nonnegative squares.
 *
 * The sequence is known to be finite; for n > 5408 every integer has
 * at least nine representations, so the largest term is ≤ 5408.
 * We precompute all counts of unordered 5‑square sums up to LIMIT
 * using nested loops over non‑increasing 5‑tuples (a ≥ b ≥ c ≥ d ≥ e ≥ 0).
 * Then we collect the integers with exactly two such representations
 * in order and answer queries from that list. */

#include <stdint.h>
#include <stdlib.h>

/* The types seq_t and idx_t are provided by the build.  No
 * definition or redefinition of them is required. */

/* maximal integer that can appear in the sequence (proof in OEIS) */
static const idx_t LIMIT = 5408;

/* compute floor(sqrt(x)) for a non‑negative unsigned integer x */
static idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) {
        ++r;
    }
    return r;
}

/* Fill the array cnt[0 .. LIMIT] with the number of unordered
 * 5‑tuple representations of each integer as a sum of 5 squares. */
static void fill_counts(uint16_t *cnt, idx_t limit)
{
    idx_t max = isqrt(limit);
    for (idx_t a = 0; a <= max; ++a) {
        idx_t sa = a * a;
        if (sa > limit) break;
        for (idx_t b = 0; b <= a; ++b) {
            idx_t sb = sa + b * b;
            if (sb > limit) break;
            for (idx_t c = 0; c <= b; ++c) {
                idx_t sc = sb + c * c;
                if (sc > limit) break;
                for (idx_t d = 0; d <= c; ++d) {
                    idx_t sd = sc + d * d;
                    if (sd > limit) break;
                    for (idx_t e = 0; e <= d; ++e) {
                        idx_t sum = sd + e * e;
                        if (sum > limit) break;
                        ++cnt[sum];
                    }
                }
            }
        }
    }
}

/* Sequence provider; memoizes the entire sequence on first call. */
seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;
    static size_t seq_len  = 0;
    static int     initialized = 0;

    if (!initialized) {
        /* Allocate array for representation counts. */
        uint16_t *cnt = (uint16_t *)calloc((size_t)LIMIT + 1, sizeof(uint16_t));
        if (!cnt) return 0;          /* allocation failed – return 0 */

        /* Count unordered 5‑square sums up to LIMIT. */
        fill_counts(cnt, LIMIT);

        /* Count how many numbers have exactly two representations. */
        size_t tmp = 0;
        for (idx_t i = 0; i <= LIMIT; ++i) {
            if (cnt[i] == 2) ++tmp;
        }

        /* Allocate the sequence array. */
        seq = (seq_t *)malloc(tmp * sizeof(seq_t));
        if (!seq) { free(cnt); return 0; }

        /* Fill the sequence with numbers having exactly two reps. */
        seq_len = 0;
        for (idx_t i = 0; i <= LIMIT; ++i) {
            if (cnt[i] == 2) {
                seq[seq_len++] = (seq_t)i;
            }
        }
        free(cnt);
        initialized = 1;
    }

    /* Bounds check: sequence is finite; return 0 for out of range. */
    if (n == 0 || n > seq_len) return 0;
    return seq[n - 1];   /* offset 1 */
}
