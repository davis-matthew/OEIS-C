/*
 * A294100  Number of partitions of n into two squarefree parts or two
 *          nonsquarefree positive integer parts.
 * https://oeis.org/A294100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i | 1 ≤ i ≤ ⌊n/2⌋ , A008966(i) = A008966(n‑i) }
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root (floor) for uint64_t.  Avoids overflow. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    static idx_t max_n = 0;
    static bool *sqfree = NULL;
    static seq_t *a_vals = NULL;

    if (n == 0) return 0;

    if (n <= max_n) return a_vals[n];

    /* Allocate new tables up to n */
    size_t sz = (size_t)(n + 1);
    bool *new_sqfree = (bool *)calloc(sz, sizeof(bool));
    seq_t *new_a_vals = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_sqfree || !new_a_vals) {
        free(new_sqfree);
        free(new_a_vals);
        return 0;
    }

    /* Initialise all as square‑free; set 0 and 1 appropriately */
    for (idx_t i = 0; i <= n; ++i) new_sqfree[i] = true;
    new_sqfree[0] = false;
    new_sqfree[1] = true;              /* 1 is square‑free */

    /* Sieve out numbers divisible by a square > 1 */
    idx_t lim = _isqrt_u64(n);
    for (idx_t p = 2; p <= lim; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= n; j += sq) {
            new_sqfree[j] = false;
        }
    }

    new_a_vals[0] = 0;
    new_a_vals[1] = 0;                 /* for n = 1, no partitions */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cnt = 0;
        idx_t half = i / 2;
        for (idx_t j = 1; j <= half; ++j) {
            if (new_sqfree[j] == new_sqfree[i - j]) {
                ++cnt;
            }
        }
        new_a_vals[i] = cnt;
    }

    /* Replace old tables with the new ones */
    free(sqfree);
    free(a_vals);
    sqfree = new_sqfree;
    a_vals = new_a_vals;
    max_n = n;

    return a_vals[n];
}
