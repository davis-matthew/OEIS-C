/*
 * A350657  a(n) is the greatest value of the orbit of n under repeated
 *          application of A350229 (the sum of a number and its balanced
 *          ternary digits).
 * https://oeis.org/A350657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the greatest value reached by iterating f(x) = x +
 *            sum_balanced_ternary_digits(x) until a cycle is reached
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t f(seq_t x)
{
    int64_t sum = 0;
    seq_t tmp = x;
    while (tmp != 0u)
    {
        seq_t r = tmp % 3u;
        tmp /= 3u;
        if (r == (seq_t)2u)
        {
            sum += -1;
            tmp += 1u;
        }
        else
        {
            sum += (int64_t)r;
        }
    }
    if (sum >= 0)
        return x + (seq_t)sum;
    else
        return x - (seq_t)(-sum);
}

seq_t a(idx_t n)
{
    seq_t max_val = (seq_t)n;

    /* Phase 1: Find meeting point of tortoise and hare */
    seq_t tortoise = f((seq_t)n);
    seq_t hare = f(f((seq_t)n));
    while (tortoise != hare)
    {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Phase 2: Find start of cycle */
    tortoise = (seq_t)n;
    while (tortoise != hare)
    {
        tortoise = f(tortoise);
        hare = f(hare);
    }
    seq_t cycle_start = tortoise;

    /* Phase 3: Compute maximum before the cycle */
    seq_t cur = (seq_t)n;
    while (cur != cycle_start)
    {
        cur = f(cur);
        if (cur > max_val) max_val = cur;
    }

    /* Phase 4: Compute maximum inside the cycle */
    seq_t cycle_max = cycle_start;
    seq_t next = f(cycle_start);
    while (next != cycle_start)
    {
        if (next > cycle_max) cycle_max = next;
        next = f(next);
    }

    if (cycle_max > max_val) max_val = cycle_max;
    return max_val;
}
