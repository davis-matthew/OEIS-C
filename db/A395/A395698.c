/*
 * A395698  Triangle read by rows: row n (n >= 1) gives the concatenated
 *          run-length encoding of the integer partitions of n, sorted first
 *          by length and then reverse-lexicographically.
 * https://oeis.org/A395698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iteratively generating the triangle rows of
   run‑length encoded integer partitions, concatenating the
   resulting integers, and returning the n‑th element in the
   flattened sequence.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Recursive generator of partitions of 'rem' into 'parts_left'
   parts, each ≤ 'max_part'.  The parts are stored in non‑increasing
   order in the array 'parts'.  When a complete partition is found,
   its run‑length encoding is emitted element by element.  The
   emitted values are counted in '*cnt'; when '*cnt' reaches
   'target', '*result' is set and the function returns true.  */
static bool gen_len_partitions(idx_t rem, int max_part, int parts_left,
                               int *parts, int pos,
                               idx_t *cnt, idx_t target, seq_t *result)
{
    if (parts_left == 0) {
        if (rem != 0) return false;          /* invalid partition */

        /* Emit the run‑length encoding of the partition. */
        int i = 0;
        while (i < pos) {
            int part = parts[i];
            int mult = 1;
            int j = i + 1;
            while (j < pos && parts[j] == part) {
                ++mult;
                ++j;
            }
            if (++(*cnt) == target) {
                *result = (seq_t)part;
                return true;
            }
            if (++(*cnt) == target) {
                *result = (seq_t)mult;
                return true;
            }
            i = j;
        }
        return false;
    }

    /* Prune impossible sums. */
    if (rem < (idx_t)parts_left) return false;
    if ((idx_t)max_part * parts_left < rem) return false;

    /* Determine the range of the next part: from high down to low. */
    int low = (int)((rem + parts_left - 1) / parts_left);    /* ceil(rem / parts_left) */
    int high = (int)((rem < (idx_t)max_part) ? rem : max_part);
    if (high < low) return false;

    for (int val = high; val >= low; --val) {
        parts[pos] = val;
        if (gen_len_partitions(rem - val, val, parts_left - 1,
                               parts, pos + 1,
                               cnt, target, result)) {
            return true;
        }
    }
    return false;
}

/* Public entry point required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, though n ≥ 1 for the sequence */

    idx_t target = n;
    idx_t count  = 0;
    seq_t result = 0;

    for (idx_t r = 1; ; ++r) {
        /* Allocate array for the largest possible partition of r. */
        size_t len = (size_t)r;
        int *parts = (int *)malloc(len * sizeof(int));
        if (!parts) return 0;             /* allocation failure */

        /* Generate partitions grouped by increasing length (#parts). */
        for (int k = 1; k <= (int)r; ++k) {
            if (gen_len_partitions((idx_t)r, (int)r, k, parts, 0,
                                   &count, target, &result)) {
                free(parts);
                return result;
            }
        }
        free(parts);
    }
}
