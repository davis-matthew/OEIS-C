/*
 * A380273  Irregular table T(n, k), n >= 0, k = 1..A380272(n), read by rows;
 *          the n-th row lists the integers m in 0..n such that the
 *          nonadjacent forms for m-n and m can be added without carries.
 * https://oeis.org/A380273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3621, u16 n <= 9839, u32 n <= 9839, u64 n <= 9839,
 *            u128 n <= 9839, big any n
 * Helpers:   naf_compute, no_carry_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A380273 sequence term a(n) by scanning the irregular
   table T(n,k).  For each row n it iterates over m = 0..n and tests whether
   (n–m) and m have nonadjacent forms that can be added without carries.
   The first m found at the global index n is returned as seq_t. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/naf_compute.h"
#include "../utils/no_carry_add.h"

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    uint64_t idx = 0;      /* current global index in the flattened table */
    for (uint64_t row = 0; ; ++row) {
        for (uint64_t m = 0; m <= row; ++m) {
            if (no_carry_add(row, m)) {
                if (idx == n) {
                    return (seq_t)m;
                }
                ++idx;
            }
        }
    }
}
