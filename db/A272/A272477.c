/*
 * A272477  Numbers n that are coprime to the numerator of
 *          zeta(2*n)/(Pi^(2*n)).
 * https://oeis.org/A272477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The nth term is the nth positive integer k such that k is odd or
 *            (k is even and popcount(k) ≤ 2). Equivalently, the sequence
 *            consists of all numbers except even numbers having more than two
 *            1‑bits in binary representation
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t cand = 1; ; ++cand)
    {
        bool good;
        if (cand & 1U)                /* odd numbers always good */
            good = true;
        else if (__builtin_popcountll((unsigned long long)cand) <= 2U)
            good = true;             /* even with ≤2 1‑bits */
        else
            good = false;            /* excluded even numbers */

        if (good) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
