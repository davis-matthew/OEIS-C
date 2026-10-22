/*
 * A374494  a(n) = A373498(A373498(n)).
 * https://oeis.org/A374494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A373498(A373498(n)) where A373498 is the intra‑block
 *            permutation described in OEIS A373498
 * Range:     u8 n <= 253, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation first computes the inverse permutation
   f⁻¹ (the position of a given value) and then applies it twice,
   yielding the required sequence. */

#include <stdint.h>

/* --- helpers ----------------------------------------------------- */

/* Compute the block number L for a given index or value n.
   The block index satisfies 2*L^2 + L >= n. */
static inline idx_t find_block(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* grow hi until it covers n */
    while (((unsigned __int128)2 * hi * hi + hi) < n)
        hi <<= 1;
    /* binary search for the smallest L with 2*L^2+L >= n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 cum = (unsigned __int128)2 * mid * mid + mid;
        if (cum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Inverse of the permutation A373498: for a value v return the
   index i such that A373498(i) = v. */
static inline seq_t A373498_inv(idx_t v)
{
    /* Find the block that contains value v */
    idx_t L = find_block(v);
    /* cumulative count of elements before this block */
    idx_t prev = (L - 1) * (2 * L - 1);
    /* P value corresponding to v inside its block */
    seq_t targetP = (seq_t)v - (seq_t)prev;

    /* Pre‑compute constants for the block */
    idx_t two_l  = 2 * L;
    idx_t two_l_minus_one = 2 * L - 1;
    idx_t four_l = 4 * L;

    /* Search position k within the block (1 .. 4*L-1) that yields P = targetP */
    for (idx_t k = 1; k <= 4 * L - 1; ++k) {
        seq_t P;
        if (k < two_l_minus_one) {
            if (k & 1)            /* k odd */
                P = (seq_t)two_l + (seq_t)k + 1;
            else                  /* k even */
                P = (seq_t)two_l - (seq_t)k - 1;
        } else {
            if (k & 1)            /* k odd */
                P = (seq_t)k;
            else                  /* k even */
                P = (seq_t)four_l - (seq_t)k;
        }
        if (P == targetP)
            return (seq_t)(prev + k);   /* index where v occurs */
    }
    /* Should never happen for valid input */
    return (seq_t)(prev + 1);
}

/* --- public function --------------------------------------------- */

seq_t a(idx_t n)
{
    /* first application of the inverse permutation */
    seq_t first  = A373498_inv(n);
    /* second application */
    return A373498_inv((idx_t)first);
}
