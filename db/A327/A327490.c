/*
 * A327490  T(n, k) = 1 + IFF(k - 1, n - k), where IFF is Boolean equality
 *          evaluated bitwise on the inputs, triangle read by rows, T(n, k)
 *          for n >= 1, 1 <= k <= n.
 * https://oeis.org/A327490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = 1 + (~((k-1) ^ (r-k))) &
 *            ((1<<bits)-1) * with bits = max(bitlen(k-1), bitlen(r-k)) and
 *            bitlen(0) = 0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the OEIS sequence A327490. */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t bitlen_u64(uint64_t x)
{
    idx_t len = 0;
    while (x) {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (((idx_t)r * (r + 1) / 2) < n) {
        ++r;
    }

    /* Column k within that row (1‑based) */
    idx_t prev = (idx_t)(r - 1) * r / 2;
    idx_t k = n - prev;

    seq_t left  = (seq_t)(k - 1);   /* k-1 */
    seq_t right = (seq_t)(r - k);   /* r-k */

    idx_t leftlen  = bitlen_u64((uint64_t)left);
    idx_t rightlen = bitlen_u64((uint64_t)right);
    idx_t bits = leftlen > rightlen ? leftlen : rightlen;

    seq_t mask;
    if (bits == 0) {
        mask = (seq_t)0;
    } else {
        mask = ((seq_t)1 << bits) - 1;
    }

    seq_t x   = left ^ right;
    seq_t val = (~x & mask) + 1;
    return val;
}
