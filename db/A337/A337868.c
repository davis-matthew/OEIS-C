/*
 * A337868  Number of distinct residues of x^r (mod n), x=0..n-1, r=2, ..., n.
 * https://oeis.org/A337868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of distinct residues of x^r (mod n) for
   0 ≤ x < n and r ≥ 2 (r up to n, the upper bound is irrelevant).
   The implementation uses a brute‑force double loop, which is
   sufficient for n ≤ 10 000 and satisfies the time limits. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                          /* a(1) = 0 */

    size_t sz = (size_t)n;                  /* n is small (< 10000) */
    unsigned char *seen = (unsigned char *)calloc(sz, 1U);
    if (!seen)
        return 0;                          /* fallback on allocation failure */

    /* For each x, generate x^r for r = 2..n and mark the residues */
    for (idx_t x = 0; x < n; ++x) {
        seq_t cur = (seq_t)x;               /* x^1 */
        for (idx_t r = 2; r <= n; ++r) {
            cur = (cur * (seq_t)x) % (seq_t)n;   /* compute x^r */
            seen[(size_t)cur] = 1;
        }
    }

    /* Count distinct residues */
    seq_t cnt = 0;
    for (size_t i = 0; i < sz; ++i)
        if (seen[i]) ++cnt;

    free(seen);
    return cnt;
}
