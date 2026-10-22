/*
 * A335458  Number of normal patterns contiguously matched by the n-th
 *          composition in standard order (A066099).
 * https://oeis.org/A335458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335474(n) + 1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of normal patterns contiguously matched by the
   n-th composition in standard order (A066099).  This implementation
   follows the definition of the composition:  write n in binary,
   enumerate the positions of 1‑bits (starting at 1 for the least
   significant bit), prepend 0, take first differences, reverse the
   list, and use that as the composition parts.  All normal patterns
   are generated from all contiguous subarrays of that composition,
   normalised by their relative order, and duplicates are removed. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    uint8_t len;
    uint8_t data[64];
} Pattern;

seq_t a(idx_t n)
{
    /* Build the composition parts from n (popcount(n) parts at most) */
    uint8_t parts[64];
    size_t plen = 0;
    idx_t tmp = n;
    uint8_t last = 0;
    uint8_t pos = 1;   /* position (1‑based) of current bit */
    while (tmp) {
        if (tmp & 1) {
            uint8_t diff = (pos - last);
            parts[plen++] = diff;
            last = pos;
        }
        ++pos;
        tmp >>= 1;
    }
    /* reverse the parts */
    for (size_t i = 0; i < plen / 2; ++i) {
        uint8_t tmpv = parts[i];
        parts[i] = parts[plen - 1 - i];
        parts[plen - 1 - i] = tmpv;
    }

    /* Generate all distinct normal patterns from contiguous subarrays */
    Pattern patterns[5000];
    size_t pat_cnt = 0;

    for (size_t start = 0; start < plen; ++start) {
        for (size_t end = start; end < plen; ++end) {
            size_t slen = end - start + 1;
            uint8_t sub[64];
            for (size_t k = 0; k < slen; ++k)
                sub[k] = parts[start + k];

            /* build list of distinct values in subarray */
            uint8_t uniq[64];
            size_t uniqcnt = 0;
            for (size_t k = 0; k < slen; ++k) {
                uint8_t v = sub[k];
                int found = 0;
                for (size_t u = 0; u < uniqcnt; ++u)
                    if (uniq[u] == v) { found = 1; break; }
                if (!found) uniq[uniqcnt++] = v;
            }
            /* sort uniq ascending */
            for (size_t i = 0; i < uniqcnt; ++i)
                for (size_t j = i + 1; j < uniqcnt; ++j)
                    if (uniq[i] > uniq[j]) {
                        uint8_t t = uniq[i]; uniq[i] = uniq[j]; uniq[j] = t;
                    }

            /* build pattern encoding */
            Pattern pat;
            pat.len = (uint8_t)slen;
            for (size_t k = 0; k < slen; ++k) {
                uint8_t v = sub[k];
                uint8_t rank = 1;
                for (size_t u = 0; u < uniqcnt; ++u) {
                    if (uniq[u] == v) { rank = (uint8_t)(u + 1); break; }
                }
                pat.data[k] = rank;
            }

            /* insert if new */
            int exists = 0;
            for (size_t p = 0; p < pat_cnt; ++p)
                if (patterns[p].len == pat.len &&
                    memcmp(patterns[p].data, pat.data, pat.len) == 0) {
                    exists = 1; break;
                }
            if (!exists) {
                patterns[pat_cnt++] = pat;
            }
        }
    }

    /* a(n) counts the empty pattern as well */
    seq_t result = (seq_t)pat_cnt + (seq_t)1;
    return result;
}
