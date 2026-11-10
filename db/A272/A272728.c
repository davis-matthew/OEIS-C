/*
 * A272728  a(n) = A272727(2n-1) - n.
 * https://oeis.org/A272728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A272727(2n-1) - n, where A272727(0)=0 and A272727(k+1) =
 *            number of j in 0..k with A272727(j) = A272727(k-j)
 * Range:     u8 n <= 849, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 111 terms through n = 6870 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A272728_cache = NULL;
static size_t A272728_len = 0;

seq_t a(idx_t n)
{
    size_t m = (size_t)(2 * n - 1);
    if (A272728_len == 0) {
        A272728_cache = mem_new(seq_t, 1);
        A272728_cache[0] = 0;
        A272728_len = 1;
    }
    if (m >= A272728_len) {
        A272728_cache = mem_resize(seq_t, A272728_cache, m + 1);
        for (size_t i = A272728_len; i <= m; ++i) {
            /* count coincidences between seq[0..i-1] and its reverse */
            size_t cnt = 0;
            size_t half = i / 2;
            for (size_t j = 0; j < half; ++j) {
                if (A272728_cache[j] == A272728_cache[i - 1 - j]) cnt += 2;
            }
            if (i & 1) cnt++; /* middle element always matches itself */
            A272728_cache[i] = (seq_t)cnt;
        }
        A272728_len = m + 1;
    }
    return A272728_cache[m] - (seq_t)n;
}
