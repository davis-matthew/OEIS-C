/*
 * A333601  Number of integers from 0 to the triangular number A000217(n) that
 *          cannot be represented as a difference of triangular numbers from 0
 *          to n.
 * https://oeis.org/A333601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(n) - A334821(n) + 1 where A000217(n) = n*(n+1)/2
 *            and A334821(n) counts the distinct differences of triangular
 *            numbers up to n
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* T(n) in seq_t and idx_t (for arithmetic and array sizes) */
    idx_t Ti = (idx_t)n * ((idx_t)n + 1) / 2;
    seq_t Tn = (seq_t)Ti;            /* triangular number */

    /* bitset for [0 .. T(n)] :: 1 bit per number */
    size_t bits_len_bits = (size_t)Ti + 1;
    size_t bytes = (bits_len_bits + 7) / 8;
    uint8_t *bits = (uint8_t *)malloc(bytes);
    if (!bits) return 0;            /* allocation failed */
    memset(bits, 0, bytes);

    /* precompute triangular numbers T(i) for i = 0..n */
    seq_t *tri = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tri) { free(bits); return 0; }
    tri[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        tri[i] = tri[i - 1] + (seq_t)i;   /* T(i) = T(i-1)+i */

    /* count distinct representable differences */
    seq_t rep = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t ti = tri[i];
        for (idx_t j = 0; j <= i; ++j) {
            seq_t diff = ti - tri[j];        /* 0 <= diff <= Tn */
            idx_t  idx  = (idx_t)diff;       /* cast to index type */
            uint8_t mask = 1u << (idx & 7);
            if (!(bits[idx >> 3] & mask)) {
                bits[idx >> 3] |= mask;
                ++rep;
            }
        }
    }

    free(tri);
    free(bits);

    /* a(n) = T(n) - rep + 1 */
    seq_t result = Tn - rep + 1;
    return result;
}
