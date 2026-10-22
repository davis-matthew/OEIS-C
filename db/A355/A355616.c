/*
 * A355616  a(n) is the number of distinct lengths between consecutive points
 *          of the Farey sequence of order n.
 * https://oeis.org/A355616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A355616:
   a(n) = number of distinct lengths between consecutive points
   of the Farey sequence of order n.
   The length between fraction a/b < c/d is 1/(b*d),
   so this is the count of distinct products b*d among consecutive
   fractions in the Farey sequence.  The function generates the
   Farey sequence of order n and records each product in a bitset
   to count the distinct values.  For n ≤ 10000 the maximum
   product is n² ≤ 1e8, which fits into a 32‑bit index. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at n = 1 */

    /* size of the bitset: one bit per product 1 … n*n */
    size_t max_prod = (size_t)n * (size_t)n;   /* ≤ 1e8 for n ≤ 10000 */
    size_t bits_len = (max_prod + 63) >> 6;    /* number of 64‑bit words */

    uint64_t *bs = (uint64_t*)calloc(bits_len, sizeof(uint64_t));
    if (!bs) return 0;                          /* allocation failure */

    /* first fraction 0/1 and second 1/n (if n>0) */
    uint64_t a_num = 0, a_den = 1;
    uint64_t c_num = 1, c_den = n;

    /* record product of the first pair: 1 * n */
    uint64_t prod = a_den * c_den;              /* = n */
    size_t idx = prod >> 6;
    bs[idx] |= ((uint64_t)1) << (prod & 63);

    uint64_t count = 1;

    while (!(c_num == 1 && c_den == 1)) {
        /* next fraction e/f */
        uint64_t k = (n + a_den) / c_den;
        uint64_t e = k * c_num - a_num;
        uint64_t f = k * c_den - a_den;

        /* product of current and next denominators */
        prod = c_den * f;
        idx = prod >> 6;
        uint64_t mask = ((uint64_t)1) << (prod & 63);

        if (!(bs[idx] & mask)) {
            bs[idx] |= mask;
            ++count;
        }

        /* advance */
        a_num = c_num; a_den = c_den;
        c_num = e;    c_den = f;
    }

    free(bs);
    return (seq_t)count;
}
