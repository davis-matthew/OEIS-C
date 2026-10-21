/*
 * A256913  Enhanced squares representations for k = 0, 1, 2, ...,
 *          concatenated.
 * https://oeis.org/A256913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf, nice
 * Formula:   a(n) = n-th term of the concatenation of the decreasing enhanced
 *            squares representation of each integer k = 0,1,2,... (OEIS
 *            A256913). The representation of k is obtained by repeatedly
 *            subtracting the largest element of B = {0,1,2,3,4,9,16,25,...}
 *            that is strictly smaller than the previous subtrac...
 * Range:     u8 n <= 706, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64-bit x. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;          /* 2^62 = floor(sqrt(2^64 - 1)) */
    while (bit != 0) {
        idx_t next = r + bit;
        if (next <= x / next) {
            r = next;
        }
        bit >>= 1;
    }
    return r;
}

/* Find the next element of the decreasing enhanced squares representation.
   rem  : remaining amount to represent (rem > 0)
   prev : maximum allowed value for the next element (strictly less than prev)
   Returns the chosen element (always in B and < prev). */
static idx_t find_base(idx_t rem, idx_t prev)
{
    /* Largest square <= rem and < prev */
    idx_t s = isqrt_u64(rem);
    idx_t square = s * s;
    while (square >= prev && s > 0) {
        --s;
        square = s * s;
    }
    idx_t base = square;

    /* Check the extra elements 3, 2, 1 (and 0 when rem == 0, but rem>0 here) */
    if (rem >= 3 && (idx_t)3 < prev && (idx_t)3 > base) base = 3;
    if (rem >= 2 && (idx_t)2 < prev && (idx_t)2 > base) base = 2;
    if (rem >= 1 && (idx_t)1 < prev && (idx_t)1 > base) base = 1;

    return base;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t pos = 0;                     /* current position in concatenated list */

    for (idx_t k = 0; ; ++k) {
        if (k == 0) {                  /* special case: 0 */
            if (pos == n) return (seq_t)0;
            ++pos;
            continue;
        }

        idx_t rem = k;
        idx_t prev = k + 1;            /* ensures the first element can be ≤ k */

        while (rem > 0) {
            idx_t elem = find_base(rem, prev);
            if (pos == n) return (seq_t)elem;
            ++pos;
            rem -= elem;
            prev = elem;                /* strictly decreasing requirement */
        }
    }
}
