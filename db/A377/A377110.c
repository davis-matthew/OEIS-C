/*
 * A377110  a(n) = coefficient of sqrt(2) in the expansion of (2 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of sqrt(2) in expansion of (2 + sqrt(2) +
 *            sqrt(3))^n Recurrence: a(n) = 8*a(n-1) - 14*a(n-2) - 8*a(n-3) +
 *            23*a(n-4) initial terms: a(0)=0, a(1)=1, a(2)=4, a(3)=23
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t cache[10001];
static idx_t computed = 0;

seq_t a(idx_t n)
{
    /* If n is beyond the pre‑allocated range, fall back to a simple
       on‑demand computation.  The problem statement expects n up to
       about 10 000, so this path is only a safeguard. */
    if (n >= 10001) {
        idx_t m = n + 1;
        seq_t *ext = (seq_t *)malloc(m * sizeof(seq_t));
        if (!ext) abort();
        ext[0] = (seq_t)0;
        ext[1] = (seq_t)1;
        ext[2] = (seq_t)4;
        ext[3] = (seq_t)23;
        for (idx_t i = 4; i <= n; ++i) {
            ext[i] = 8 * ext[i-1] - 14 * ext[i-2] - 8 * ext[i-3] + 23 * ext[i-4];
        }
        seq_t res = ext[n];
        free(ext);
        return res;
    }

    /* Initialise the static cache if this is the first call. */
    if (computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)23;
        computed = 4;
    }

    /* Compute any missing terms up to n. */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            cache[i] = 8 * cache[i-1] - 14 * cache[i-2] - 8 * cache[i-3] + 23 * cache[i-4];
        }
        computed = n + 1;
    }

    return cache[n];
}
