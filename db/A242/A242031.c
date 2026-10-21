/*
 * A242031  Numbers n such that prime factorization n =
 *          p_1^k_1*p_2^k_2*...*p_r^k_r satisfies k_1 >= k_2 >= ... >= k_r.
 * https://oeis.org/A242031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponents_nonincreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n‑th term of OEIS A242031.
 * 
 * The sequence consists of all positive integers whose prime‑factor
 * exponents form a non‑increasing sequence.  The implementation
 * simply scans integers in increasing order and keeps those that
 * satisfy the condition until the desired index is reached.
 *
 * The code is written to be valid C11 and C++17 alike.  It makes
 * no assumptions about the order of calls and requires no
 * pre‑built tables.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/exponents_nonincreasing.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t result = 0;

    for (uint64_t cand = 1; count < n; ++cand)
    {
        if (exponents_nonincreasing(cand))
        {
            ++count;
            if (count == n)
                result = (seq_t)cand;  /* cast to the desired seq_t type */
        }
    }
    return result;
}
