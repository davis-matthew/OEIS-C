/*
 * A384219  Areas of triangles whose three vertices are consecutive ordered
 *          pairs of consecutive odd Fibonacci numbers such that an ordered
 *          pair's y-value is the next ordered pair's x-value.
 * https://oeis.org/A384219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs((b-a)*(d-b) - (c-b)*(c-a)) / 2 where
 *            (a,b),(b,c),(c,d) are consecutive ordered pairs of consecutive
 *            odd Fibonacci numbers
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __SIZEOF_INT128__
    typedef unsigned __int128 u128;
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* indices below offset */

    /* static cache of odd Fibonacci numbers */
    static seq_t *oddFib = NULL;
    static size_t oddCap = 0;
    static size_t oddCount = 0;

    /* static state for generating Fibonacci numbers */
    static idx_t fibIndex   = 2;          /* currently at F2 = 1 */
    static seq_t fibPrevPrev = 1;        /* F1 = 1 */
    static seq_t fibPrev     = 1;        /* F2 = 1 */

    /* initialise cache on first call */
    if (!oddFib) {
        oddCap = 4;
        oddFib = (seq_t *)realloc(NULL, oddCap * sizeof(seq_t));
        if (!oddFib) return 0;
        oddFib[0] = 1;                  /* first odd Fibonacci number F1 */
        oddFib[1] = 1;                  /* second odd Fibonacci number F2 */
        oddCount = 2;
    }

    /* Ensure we have at least n + 3 odd Fibonacci numbers */
    size_t needed = (size_t)n + 3;
    while (oddCount < needed) {
        /* grow the array if needed */
        if (oddCount >= oddCap) {
            size_t newCap = oddCap * 2;
            seq_t *tmp = (seq_t *)realloc(oddFib, newCap * sizeof(seq_t));
            if (!tmp) return 0;
            oddFib = tmp;
            oddCap = newCap;
        }

        /* generate next Fibonacci number */
        fibIndex++;
        seq_t newFib = fibPrev + fibPrevPrev;
        fibPrevPrev = fibPrev;
        fibPrev = newFib;

        /* store if odd */
        if ((newFib & 1) != 0) {
            oddFib[oddCount++] = newFib;
        }
    }

    /* Fetch the four consecutive odd Fibonacci numbers */
    seq_t a0 = oddFib[(size_t)n - 1];
    seq_t b0 = oddFib[(size_t)n];
    seq_t c0 = oddFib[(size_t)n + 1];
    seq_t d0 = oddFib[(size_t)n + 2];

#ifdef __SIZEOF_INT128__
    /* When seq_t fits in 8 bytes, use 128-bit intermediate to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        u128 prod1 = (u128)(b0 - a0) * (u128)(d0 - b0);
        u128 prod2 = (u128)(c0 - b0) * (u128)(c0 - a0);
        u128 diff  = prod1 > prod2 ? prod1 - prod2 : prod2 - prod1;
        return (seq_t)(diff / 2);
    }
#endif
    /* Generic computation for larger seq_t (e.g., arbitrary‑precision) */
    seq_t prod1 = (b0 - a0) * (d0 - b0);
    seq_t prod2 = (c0 - b0) * (c0 - a0);
    seq_t diff  = prod1 > prod2 ? prod1 - prod2 : prod2 - prod1;
    return diff / 2;
}
