/*
 * A357616  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, the number of 1's in the
 *          ternary expansion of n equals the number of 2's in the ternary
 *          expansion of a(n) and vice versa.
 * https://oeis.org/A357616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically earliest permutation of the
 *            nonnegative integers such that #1's in n = #2's in a(n) and vice
 *            versa
 * Range:     u8 n <= 242, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   _C, _build_from_rank, _count_suffix, _count_total, _rank_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types are provided elsewhere */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_C.h"
#include "../utils/_build_from_rank.h"
#include "../utils/_count_suffix.h"
#include "../utils/_count_total.h"
#include "../utils/_rank_set.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Count ones and twos in ternary representation of n */
    int c1 = 0, c2 = 0;
    idx_t t = n;
    while (t > 0) {
        int d = t % 3;
        if (d == 1) c1++;
        else if (d == 2) c2++;
        t /= 3;
    }

    /* Compute the rank of n among numbers having (c1,c2) */
    uint64_t rank = _rank_set(n, c1, c2);

    /* Find the number with the same rank among numbers with (c2,c1) */
    seq_t res = _build_from_rank(rank, c2, c1);
    return res;
}
