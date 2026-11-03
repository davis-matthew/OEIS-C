/*
 * A356842  Numbers k such that the k-th composition in standard order does
 *          not cover an interval of positive integers (not gapless).
 * https://oeis.org/A356842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer k such that
 * the k-th composition in the standard order (A066099) has a set of
 * part sizes that is not a contiguous interval of positive integers.
 * The k-th composition is obtained from k by:
 *   * take the positions of 1's in the reversed binary expansion of k
 *     (positions counted from 1 at the least‑significant bit),
 *   * prepend 0,
 *   * take first differences,
 *   * reverse the list of differences.
 * This gives a bijection between nonnegative integers and all
 * compositions.
 *
 * The algorithm below simply enumerates k starting from 1, builds the
 * composition using the above rule, checks whether its part sizes
 * form a contiguous interval, and collects the first n values that
 * do NOT form such an interval.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* helper: build the composition for index k into parts[0..len-1]   */
static void compose_from_index(idx_t k, idx_t parts[65], size_t *len)
{
    /* collect positions of 1 bits (bit 1 for LSB) */
    idx_t pos[65];
    size_t poscnt = 0;
    size_t bitpos = 1;
    idx_t tmp = k;

    while (tmp != 0) {
        if (tmp & 1ULL)
            pos[poscnt++] = bitpos;
        tmp >>= 1;
        bitpos++;
    }

    /* prepend 0 implicitly by starting diff from 0 */
    size_t partslen = poscnt;           /* number of differences */
    *len = partslen;
    idx_t prev = 0;

    for (size_t i = 0; i < poscnt; ++i) {
        parts[i] = pos[i] - prev;       /* first difference */
        prev = pos[i];
    }

    /* reverse the differences to obtain the composition */
    for (size_t i = 0; i < partslen / 2; ++i) {
        idx_t tmpv = parts[i];
        parts[i] = parts[partslen - 1 - i];
        parts[partslen - 1 - i] = tmpv;
    }
}

/* helper: test if the part-size set is NOT a contiguous interval */
static bool not_gapless(const idx_t parts[65], size_t len)
{
    if (len == 0) return false;

    idx_t minp = parts[0], maxp = parts[0];
    for (size_t i = 1; i < len; ++i) {
        idx_t p = parts[i];
        if (p < minp) minp = p;
        if (p > maxp) maxp = p;
    }

    size_t span = (size_t)(maxp - minp + 1);
    if (span > 65) return true; /* safety, but not expected */

    bool present[65] = {false};
    for (size_t i = 0; i < len; ++i)
        present[parts[i] - minp] = true;

    for (size_t i = 0; i < span; ++i)
        if (!present[i]) return true;      /* missing integer */

    return false;                          /* contiguous set */
}

/* the required function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    idx_t parts[65];
    size_t len;

    while (count < n) {
        compose_from_index(k, parts, &len);
        if (not_gapless(parts, len)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
    /* unreachable, but to silence warnings */
    return (seq_t)0;
}
