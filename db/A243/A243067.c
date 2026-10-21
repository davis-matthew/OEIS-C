/*
 * A243067  Integers from 0 to A000120(n)-1 followed by integers from 0 to
 *          A000120(n+1)-1 and so on, starting with n=1.
 * https://oeis.org/A243067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A243067: integers from 0 to popcount(n)-1 concatenated
 * block by block (offset 1).  The term a(n) is the element in the
 * block containing the n-th position, where the block for integer k
 * contains the numbers 0 .. popcount(k)-1.
 * 
 * Implementation: compute cumulative sums of popcounts until
 * reaching the block that contains n, then compute the local offset.
 * 
 * Complexity: O(m) where m is the block index, which is O(n / log n)
 * for n up to 10 000 (worst case ~2000 iterations).
 */

#include <stdint.h>

static inline unsigned int popcount_idx(idx_t x) {
    /* portable popcount for 64‑bit indices */
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    /* n is 1‑based */
    idx_t block = 1;          /* current block (the integer being repeated) */
    idx_t cum   = 0;          /* cumulative count of elements before this block */

    while (1) {
        unsigned int pc = popcount_idx(block);   /* popcount of current block */
        if (cum + pc >= n)                       /* target is inside this block */
            break;
        cum += pc;                               /* advance to next block */
        ++block;
    }

    /* local offset within the block, zero‑based */
    return (seq_t)(n - cum - 1);
}
