/*
 * A384436  a(n) is the number of distinct ways to represent n in any integer
 *          base >= 2 using only square digits.
 * https://oeis.org/A384436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A384436: number of distinct ways to represent n in any integer base >= 2
    using only square digits.
    The count is the number of bases b >= 2 for which every digit of n expressed
    in base b is a perfect square (0,1,4,9,16, …).  For n = 0 or 1 the answer
    is defined to be 1 as per the OEIS definition. */

/*  This implementation runs in O(n) time for a given n and uses only integer
    arithmetic.  It does not depend on any external helpers.  */
#include <stdbool.h>
#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit integers.  */
static idx_t floor_sqrt(idx_t x)
{
    if (x < 2)
        return x;               /* sqrt(0)=0, sqrt(1)=1 */
    idx_t low  = 1;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* avoid overflow in mid*mid by using division */
        if (mid > x / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;                 /* high will be floor(sqrt(x)) */
}

/* Test whether an unsigned integer is a perfect square.  */
static bool is_square_idx(idx_t v)
{
    if (v < 2)
        return true;             /* 0 and 1 are squares */
    idx_t r = floor_sqrt(v);
    return r * r == v;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* OEIS offset: a(0) = a(1) = 1. */
    if (n <= 1)
        return (seq_t)1;

    seq_t count = 0;

    /* The representation [n] in all bases > n contributes once
       iff n itself is a square digit. */
    if (is_square_idx(n))
        count = 1;

    /* Check all bases b from 2 up to n.  Base n is included and
       automatically produces the representation [1,0], which is valid. */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t temp = n;
        bool ok = true;
        while (temp > 0) {
            idx_t digit = temp % b;
            if (!is_square_idx(digit)) {
                ok = false;
                break;
            }
            temp /= b;
        }
        if (ok)
            ++count;
    }

    return count;
}
