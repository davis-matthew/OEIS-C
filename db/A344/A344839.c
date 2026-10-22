/*
 * A344839  Square array T(n, k), n, k >= 0, read by antidiagonals; T(n, k) =
 *          abs(n * 2^max(0, w(k)-w(n)) - k * 2^max(0, w(n)-w(k))) (where w =
 *          A070939).
 * https://oeis.org/A344839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = read the square array T(i,j) by antidiagonals; T(i,j) =
 *            |i * 2^max(0,bitlen(j)-bitlen(i)) - j *
 *            2^max(0,bitlen(i)-bitlen(j))| (where bitlen(x) = number of bits
 *            needed to represent x, 0↦0)
 * Range:     u8 n <= 9720, u16 n <= 9720, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   antidiagonal_coords, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/pow2.h"

static inline unsigned bitlen_idx(idx_t x)
{
    return x == 0 ? 0U : 64 - __builtin_clzll((uint64_t)x);
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    unsigned bi = bitlen_idx(i);
    unsigned bj = bitlen_idx(j);
    int diff = (int)bj - (int)bi;

    seq_t term1, term2;
    if (diff > 0)
    {
        term1 = ((seq_t)i) * pow2((idx_t)diff);
        term2 = (seq_t)j;
    }
    else if (diff < 0)
    {
        term1 = ((seq_t)j) * pow2((idx_t)(-diff));
        term2 = (seq_t)i;
    }
    else
    {
        term1 = (seq_t)i;
        term2 = (seq_t)j;
    }

    return term1 > term2 ? term1 - term2 : term2 - term1;
}
