/*
 * A335474  Number of nonempty normal patterns contiguously matched by the
 *          n-th composition in standard order.
 * https://oeis.org/A335474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct weak‑order patterns of all * nonempty
 *            contiguous subarrays of the n‑th composition in standard * order
 *            (graded reverse‑lexicographic). The composition of n is *
 *            obtained from the binary representation of n as described in the
 *            OEIS * entry. The pattern of a subarray is th...
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/cmp_seq_t.h"   /* for qsort comparison of seq_t */

seq_t a(idx_t n)
{
    /* n = 0 has no composition – return the forced value 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* ---------- 1. Build the composition of n. ---------- */
    /* The n-th composition is determined from the positions of 1‑bits
     * in the binary representation of n.  We collect the positions,
     * prepend 0, take first differences, and reverse the result. */
    int pos[65];          /* positions of 1‑bits, plus the leading 0 */
    int poscnt = 1;
    pos[0] = 0;           /* prepend 0 */
    idx_t tmp = n;
    int bitpos = 1;
    while (tmp > 0) {
        if (tmp & (idx_t)1) {
            pos[poscnt++] = bitpos;
        }
        tmp >>= 1;
        ++bitpos;
    }
    /* At this point, pos[0] = 0 and the remaining pos[1..poscnt-1]
     * are the 1‑bit positions from least‑significant upwards.  The
     * composition has length len = poscnt-1. */
    int len = poscnt - 1;          /* number of parts */
    int parts[16];                 /* length is at most 64; for n≤10000, len≤14 */
    for (int i = 1; i < poscnt; ++i) {
        parts[i - 1] = pos[i] - pos[i - 1];
    }
    /* Reverse the parts array to obtain the final composition order. */
    for (int i = 0; i < len / 2; ++i) {
        int tmpv = parts[i];
        parts[i] = parts[len - 1 - i];
        parts[len - 1 - i] = tmpv;
    }

    /* ---------- 2. Enumerate all contiguous subarrays. ---------- */
    /* There are at most len*(len+1)/2 subarrays; for n≤10000 this is <=
     * 105.  We collect the pattern codes in a static array, then
     * deduplicate them. */
    static const size_t MAX_PATTERNS = 512;   /* ample room for all subarrays */
    seq_t patterns[MAX_PATTERNS];
    size_t patlen = 0;

    for (int start = 0; start < len; ++start) {
        /* For each new start we maintain a dynamic set of distinct values
         * in the current subarray; it will be updated as we extend the end
         * index.  The set is kept unsorted; it will be sorted once per
         * subarray to compute ranks. */
        int uniq[16];
        int uniqcnt = 0;
        for (int end = start; end < len; ++end) {
            int val = parts[end];
            bool already = false;
            for (int u = 0; u < uniqcnt; ++u) {
                if (uniq[u] == val) {
                    already = true;
                    break;
                }
            }
            if (!already) {
                uniq[uniqcnt++] = val;
            }

            /* Sort the unique values ascending to obtain ranks. */
            int sorted[16];
            for (int u = 0; u < uniqcnt; ++u) {
                sorted[u] = uniq[u];
            }
            for (int u = 1; u < uniqcnt; ++u) {
                int key = sorted[u];
                int v = u - 1;
                while (v >= 0 && sorted[v] > key) {
                    sorted[v + 1] = sorted[v];
                    --v;
                }
                sorted[v + 1] = key;
            }

            /* Compute the pattern code for the subarray [start..end]. */
            seq_t code = 0;
            seq_t mult = 1;
            for (int i = start; i <= end; ++i) {
                int x = parts[i];
                /* Find rank of x in sorted[0..uniqcnt-1]. */
                int rank = 0;
                for (int r = 0; r < uniqcnt; ++r) {
                    if (sorted[r] == x) {
                        rank = r + 1;   /* ranks start at 1 */
                        break;
                    }
                }
                code += (seq_t)rank * mult;
                mult *= (seq_t)16;     /* base > max possible rank (≤len) */
            }
            patterns[patlen++] = code;
        }
    }

    /* ---------- 3. Count unique patterns. ---------- */
    size_t uniqcnt = 0;
    if (patlen > 0) {
        qsort(patterns, patlen, sizeof(seq_t), cmp_seq_t);
        uniqcnt = 1;               /* at least one pattern */
        for (size_t i = 1; i < patlen; ++i) {
            if (patterns[i] != patterns[i - 1]) {
                ++uniqcnt;
            }
        }
    }

    return (seq_t)uniqcnt;
}
