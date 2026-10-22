/*
 * A327489  T(n, k) = 1 + NOR(k - 1, n - k), where NOR is the Peirce arrow
 *          operating bitwise on the inputs, triangle read by rows, T(n, k)
 *          for n >= 1, 1 <= k <= n.
 * https://oeis.org/A327489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + NOR(k-1, n-k) where k = n - (r-1)r/2 and r is the
 *            smallest integer such that r(r+1)/2 >= n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/bitlen_seq_t.h"

/* Compute bitwise NOR of a and b using the minimal number of bits
   required to represent either operand. */
static inline seq_t nor_bits(seq_t a, seq_t b)
{
    size_t wa = bitlen_seq_t(a);
    size_t wb = bitlen_seq_t(b);
    size_t width = wa > wb ? wa : wb;
    seq_t mask;

    if (width == 0) {
        mask = (seq_t)0;
    } else if (width >= sizeof(seq_t) * CHAR_BIT) {
        mask = ~((seq_t)0);
    } else {
        mask = (((seq_t)1 << width) - (seq_t)1);
    }
    return (~(a | b)) & mask;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */

    /* Find the row r using binary search. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Compute the column index k (1‑based). */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;   /* 1 <= k <= r */

    seq_t x = (seq_t)(k - 1);     /* k-1 */
    seq_t y = (seq_t)(r - k);     /* n-k */

    seq_t nor = nor_bits(x, y);
    return (seq_t)1 + nor;
}
