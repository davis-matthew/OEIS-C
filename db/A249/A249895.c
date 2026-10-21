/*
 * A249895  a(n+1) is next smallest square not divisible by 10 beginning with
 *          a(n), initial term is 5.
 * https://oeis.org/A249895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements OEIS sequence A249895:
 * a(1) = 5
 * a(n+1) = the smallest perfect square > a(n) that starts with the decimal
 *          digits of a(n) and is not divisible by 10.
 * The implementation iteratively generates terms by searching for the
 * next square in increasing length prefixes.
 */
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Return the next square satisfying the sequence definition */
static seq_t next_square(seq_t prev) {
    seq_t pow10 = (seq_t)1;          /* 10^k for increasing k */
    for (;;) {
        /* interval [prev*pow10 , (prev+1)*pow10)  */
        seq_t start  = prev * pow10;
        seq_t limit  = (prev + (seq_t)1) * pow10;   /* exclusive upper bound */

        /* ceil(sqrt(start)) */
        seq_t low = floor_sqrt(start);
        if (low * low < start)
            low += (seq_t)1;

        /* ensure the square is strictly greater than prev */
        if (low * low <= prev)
            low += (seq_t)1;

        for (;; ) {
            seq_t square = low * low;
            if (square >= limit)                /* no square with this prefix */
                break;
            if (square % (seq_t)10 != (seq_t)0)  /* not divisible by 10 */
                return square;
            low += (seq_t)1;
        }

        /* try a longer prefix (next power of 10) */
        pow10 *= (seq_t)10;
    }
}

/* Public function returning the n-th term (1‑based index) */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                      /* undefined for n < 1 */
    seq_t cur = (seq_t)5;                     /* a(1) */
    idx_t i;
    for (i = 1; i < n; ++i) {
        cur = next_square(cur);
    }
    return cur;
}
