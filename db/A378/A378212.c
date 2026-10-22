/*
 * A378212  a(n) is the greatest nonnegative integer k such that there exists
 *          a strictly increasing integer sequence k = b_1 < b_2 < ... < b_t =
 *          n with the property that b_1 XOR b_2 XOR ... XOR b_t = 0, or 0 if
 *          there are no such k (when n is a power of 2).
 * https://oeis.org/A378212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 9684, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   insert_into_basis, representable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378212: a(n) is the greatest starter k such that there exists a strictly increasing
 * integer sequence k = b1 < b2 < … < bt = n with b1 XOR b2 XOR … XOR bt = 0, or 0 if no such sequence
 * exists (when n is a power of two).  This implementation computes a(n) by searching downwards
 * from n-1 for the largest k whose target XOR k ^ n can be represented by a subset of the
 * intermediate integers {k+1, … , n-1}.  It uses a linear‑basis over GF(2) to test representability.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/insert_into_basis.h"
#include "../utils/representable.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return (seq_t)0;
    if ((n & (n - 1)) == 0) return (seq_t)0;   /* n is a power of two */

    uint64_t basis[64] = {0};

    /* Search for the largest starter k (k < n) with a solvable XOR condition */
    for (idx_t k = n - 1; k > 0; --k) {
        uint64_t target = k ^ n;
        if (representable(target, basis)) return (seq_t)k;
        insert_into_basis(k, basis);    /* add current k for the next iteration */
    }

    /* After the loop, basis contains all numbers from 1 to n-1 */
    if (representable(n, basis)) return (seq_t)0;   /* k = 0 works */
    return (seq_t)0;   /* safe fallback */
}
