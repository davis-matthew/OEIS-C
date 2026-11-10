/*
 * A353305  The smallest number reached after n when iterating map x ->
 *          A353313(x) and starting from x=n, or -1 if no finite cycle is ever
 *          reached.
 * https://oeis.org/A353305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min of the trajectory n -> A353313(n) -> ... (all terms
 *            after n, including the whole terminal cycle), found by Brent
 *            cycle detection; -1 if no cycle is found (search bounded by step
 *            count / magnitude)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 91 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A353305_step(seq_t x)
{
    seq_t k = x / 3;
    seq_t r = x % 3;
    if (r == 0)
        return k;
    return 5 * k + 3 + r;
}

seq_t a(idx_t n)
{
    seq_t minus1 = (seq_t)0 - 1;
    bool is_big = (minus1 < (seq_t)0);   /* arbitrary precision: -1 is negative */
    seq_t maxv;
    if (is_big) {
        maxv = 1;
        for (int i = 0; i < 4000; i++)
            maxv *= 2;
    } else {
        maxv = minus1 / 6;
    }

    seq_t tort = (seq_t)n;
    if (tort > maxv)
        return minus1;
    seq_t hare = A353305_step(tort);
    seq_t best = hare;
    unsigned long long power = 1, lam = 1;
    unsigned long long steps = 0;
    while (tort != hare) {
        if (power == lam) {
            tort = hare;
            power *= 2;
            lam = 0;
        }
        if (hare > maxv)
            return minus1;
        hare = A353305_step(hare);
        lam++;
        steps++;
        if (hare < best)
            best = hare;
        if (steps > 20000000ULL)
            return minus1;
    }
    return best;
}
