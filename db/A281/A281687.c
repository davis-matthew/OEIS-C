/*
 * A281687  Number of partitions of 2*n into the sum of two totient numbers
 *          (A002202).
 * https://oeis.org/A281687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 1363, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of 2*n into the sum of two totient numbers
 * (A002202).  Each partition is an unordered pair of distinct
 * Euler‐totient values whose sum is 2*n.  The same totient value
 * may be used twice (e.g. 6+6). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for Euler's totient table */
static idx_t *phi_tbl = NULL;
static idx_t   phi_lim = 0;

/* Build or extend the totient table up to 'need'.
   The table is kept in ascending index order:
   phi_tbl[i] == φ(i). */
static void
ensure_phi(idx_t need)
{
    if (need <= phi_lim) return;

    /* Allocate a fresh table; old contents can be discarded. */
    idx_t *new_tbl = (idx_t *)calloc(need + 1, sizeof(idx_t));
    if (!new_tbl) return;                 /* out of memory: should never happen */

    for (idx_t i = 0; i <= need; ++i) new_tbl[i] = i;

    for (idx_t i = 2; i <= need; ++i) {
        if (new_tbl[i] == i) {            /* i is prime */
            for (idx_t j = i; j <= need; j += i)
                new_tbl[j] -= new_tbl[j] / i;
        }
    }

    free(phi_tbl);
    phi_tbl = new_tbl;
    phi_lim = need;
}

/* a(n) as described above.  GNU C++17 and C11 are both supported. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* defined only for n>=1 */

    /* 2*n is guaranteed to fit in idx_t for the intended test range. */
    idx_t target = 2 * n;

    /* Safety against overflow of idx_t. */
    if (target > (UINT64_MAX - 2) / 4)
        target = UINT64_MAX / 4 - 1;           /* max safe limit */

    idx_t limit = target * 4 + 2;              /* enough to hit every totient <= target */
    if (limit < target) limit = target;        /* in case of overflow */

    ensure_phi(limit);

    /* Boolean array marking which totient values ≤ target exist. */
    bool *seen = (bool *)calloc((size_t)(target + 1), sizeof(bool));
    if (!seen) return 0;                      /* out of memory: should never happen */

    for (idx_t i = 1; i <= limit; ++i) {
        idx_t v = phi_tbl[i];
        if (v <= target) seen[v] = true;
    }

    seq_t count = 0;
    for (idx_t v = 1; v <= target / 2; ++v) {
        if (seen[v] && seen[target - v]) {
            /* Each unordered pair (v, target-v) counts once.
             * For v == target-v (i.e. target even and v = target/2)
             * the pair uses the same totient value twice. */
            ++count;
        }
    }

    free(seen);
    return count;
}
