/*
 * A360530  a(n) is the smallest positive integer k such that n can be
 *          expressed as the arithmetic mean of k nonzero squares.
 * https://oeis.org/A360530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that n*k is the sum of k positive
 *            squares. The search is performed for k = 1, 2, 3, 4 (theorem
 *            guarantees a(n) ≤ 4)
 * Range:     u8 n <= 21, u16 n <= 5368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t x) */

/* Test whether v can be expressed as the sum of two positive squares. */
static bool is_sum_two_positive(seq_t v)
{
    /* v is guaranteed to be a small integer (≤ 2*n ≤ 20000 for n≤10000) */
    idx_t r = (idx_t)floor_sqrt(v);
    for (idx_t a = 1; a <= r; ++a)
    {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        if (a_sq >= v)
            break;
        seq_t rem = v - a_sq;
        if (rem <= 0)
            continue;
        idx_t b = (idx_t)floor_sqrt(rem);
        if ((seq_t)b * (seq_t)b == rem && b >= 1)
            return true;
    }
    return false;
}

/* Test whether v can be expressed as the sum of three positive squares. */
static bool is_sum_three_positive(seq_t v)
{
    idx_t r = (idx_t)floor_sqrt(v);
    for (idx_t a = 1; a <= r; ++a)
    {
        seq_t a_sq = (seq_t)a * (seq_t)a;
        if (a_sq >= v)
            break;
        seq_t rem = v - a_sq;
        if (rem <= 0)
            continue;
        if (is_sum_two_positive(rem))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined; return 0 for safety */

    seq_t sn = (seq_t)n;

    /* k = 1 : n itself must be a perfect square. */
    if ((seq_t)( (idx_t)floor_sqrt(sn) ) * (seq_t)( (idx_t)floor_sqrt(sn) ) == sn)
        return 1;

    /* k = 2 : check if 2*n is sum of two positive squares. */
    seq_t v2 = sn * 2;
    if (is_sum_two_positive(v2))
        return 2;

    /* k = 3 : check if 3*n is sum of three positive squares. */
    seq_t v3 = sn * 3;
    if (is_sum_three_positive(v3))
        return 3;

    /* k = 4 : guaranteed to work by the theorem. */
    return 4;
}
