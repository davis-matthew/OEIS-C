/*
 * A342390  Numbers k such that the ring of integers of Q(2^(1/k)) is not
 *          Z[2^(1/k)].
 * https://oeis.org/A342390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest integer divisible by 1093 or 3511
 * Range:     u8 none, u16 n <= 39, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t P1 = (seq_t)1093;
static const seq_t P2 = (seq_t)3511;
static const seq_t LCM = (seq_t)1093 * (seq_t)3511;

static seq_t count_multiples(seq_t x)
{
    return x / P1 + x / P2 - x / LCM;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0, returns 0 */

    seq_t low  = 1;
    seq_t high = ((seq_t)n) * P2;  /* upper bound, ensures enough */

    /* enlarge high if not enough */
    while (count_multiples(high) < n)
        high = high * 2;

    while (low < high)
    {
        seq_t mid = (low + high) / 2;
        if (count_multiples(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
