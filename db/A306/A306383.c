/*
 * A306383  Number of ways to write n as x*(2x+1) + y*(2y+1) + z*(2z+1), where
 *          x,y,z are nonnegative integers with x <= y <= z.
 * https://oeis.org/A306383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of non‑negative integers
 *            satisfying x ≤ y ≤ z and n = x\*(2x+1) + y\*(2y+1) + z\*(2z+1)
 * Range:     u8 n <= 752, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

static seq_t *hex_cache = NULL;      /* pre‑computed hexagonal numbers */
static size_t hex_cap   = 0;          /* current capacity of hex_cache */

seq_t a(idx_t n)
{
    /* convert n to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t res  = 0;

    /* Build hexagonal numbers up to n (h(k)=k*(2k+1)) */
    idx_t k = 0;
    seq_t hk = 0;                      /* h(0) = 0 */
    while (hk <= nseq) {
        /* grow the cache if necessary */
        ensure_capacity((size_t)(k + 1), &hex_cap, (void **)&hex_cache, sizeof(seq_t));
        hex_cache[k] = hk;
        ++k;
        /* compute next hexagonal number */
        hk = (seq_t)k * ((seq_t)2 * k + 1);
    }
    /* k now points to first index where h(k) > n, so max index = k-1 */
    if (k == 0) return 0;              /* n < 0 not possible, but guard */
    idx_t maxk = k - 1;

    /* Count triples (x,y,z) with 0 ≤ x ≤ y ≤ z ≤ maxk and h(x)+h(y)+h(z)=n */
    for (idx_t x = 0; x <= maxk; ++x) {
        seq_t hx = hex_cache[x];
        for (idx_t y = x; y <= maxk; ++y) {
            seq_t hy = hex_cache[y];
            if (hx + hy > nseq)           /* no further y possible */
                break;
            seq_t rem = nseq - hx - hy;   /* remaining value for z */

            if (rem < hy)                 /* z must be ≥ y */
                continue;
            if (rem > hex_cache[maxk])    /* exceeds maximum hexagonal number */
                continue;

            /* binary search for z satisfying h(z)=rem, z≥y */
            idx_t lo = y, hi = maxk;
            while (lo <= hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                seq_t hmid = hex_cache[mid];
                if (hmid == rem) {
                    ++res;               /* found a valid triple */
                    break;               /* only one z for given x,y */
                } else if (hmid < rem) {
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
        }
    }

    return res;
}
