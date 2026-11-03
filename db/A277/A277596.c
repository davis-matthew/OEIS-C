/*
 * A277596  Numbers k such that k/10^m == 9 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth positive integer whose rightmost non‑zero decimal
 *            digit is 9. The function uses a binary search over the value
 *            space and counts how many numbers ≤ x satisfy the property
 * Range:     u8 n <= 27, u16 n <= 7279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9659 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* may be required for underlying types */

static seq_t count_up_to(seq_t x)
{
    seq_t total = 0;
    seq_t power = (seq_t)1;   /* 10^m */

    for (;;)
    {
        seq_t limit = x / power;
        if (limit == (seq_t)0)
            break;

        if (limit >= (seq_t)9)
        {
            seq_t cnt = ((limit - (seq_t)9) / (seq_t)10) + (seq_t)1;
            total += cnt;
        }

        /* avoid overflow of power while multiplying by 10 */
        if (power > x / (seq_t)10)
            break;
        power *= (seq_t)10;
    }

    return total;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;      /* value we need to reach in count */
    seq_t low    = (seq_t)0;
    seq_t high   = (seq_t)1;

    /* grow high until it surely contains the nth element */
    while (count_up_to(high) < target)
    {
        high <<= 1;              /* multiply by 2 */
        if (high == (seq_t)0)    /* protect against overflow */
            high = (seq_t)-1;
    }

    /* binary search for the smallest x with count_up_to(x) >= target */
    while (low < high)
    {
        seq_t mid = (low + high) / (seq_t)2;
        if (count_up_to(mid) >= target)
            high = mid;
        else
            low  = mid + (seq_t)1;
    }

    return low; /* this is a(n) */
}
