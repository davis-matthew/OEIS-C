/*
 * A335484  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (3,2,1).
 * https://oeis.org/A335484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that the k-th composition
 * (graded reverse-lexicographic order, A066099) contains a strictly
 * decreasing subsequence of length 3 – i.e. the pattern (3,2,1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper: does the k-th composition contain a decreasing triple?  */
static bool has_decreasing_triple(idx_t k)
{
    if (k == 0)
        return false;

    /* gather the part sizes by scanning reversed binary */
    int parts[64];
    int m = 0;
    idx_t temp = k;
    int idx = 0;
    int prev = 0;     /* position of previous 1, start with 0 */
    while (temp) {
        if (temp & 1) {
            int pos = idx + 1;          /* positions start at 1 */
            parts[m++] = pos - prev;    /* difference gives part size */
            prev = pos;
        }
        temp >>= 1;
        idx++;
    }
    if (m < 3)
        return false;   /* need at least 3 parts to form a decreasing triple */

    /* compute longest decreasing subsequence length on the reversed parts.
     * We use the LIS algorithm on the negative values of parts to find a
     * strictly decreasing subsequence.  We only need to know if length >=3. */
    int tails[64];
    int len = 0;
    for (int i = m - 1; i >= 0; i--) {
        int val = -parts[i];          /* transform to increasing subsequence */
        int low = 0, high = len - 1, pos = len;
        while (low <= high) {
            int mid = (low + high) >> 1;
            if (tails[mid] < val) {   /* find first >= val */
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        pos = low;
        tails[pos] = val;
        if (pos == len)
            len++;
        if (len >= 3)
            return true;
    }
    return false;
}

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t next_k = 1;    /* next k to examine */

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 0 */
        return 0;

    /* extend cache until we have at least n entries */
    while (cache_size < n) {
        while (true) {
            if (has_decreasing_triple(next_k)) {
                /* grow cache if necessary */
                if (cache_size == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_k;
                next_k++;
                break;   /* found the next term */
            }
            next_k++;
        }
    }
    return cache[n - 1];
}
