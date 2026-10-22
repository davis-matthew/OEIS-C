/*
 * A379510  a(n) = n + floor(n*r/t) + floor(n*s/t), where r=2^(1/4),
 *          s=2^(1/2), t=2^(3/4).
 * https://oeis.org/A379510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/w⌋ + ⌊n/w²⌋, where w = 2^(1/4). This is equivalent
 *            to a(n) = n + floor( (n⁴ / 2)^(1/4) ) + floor( (n⁴ / 4)^(1/4) )
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 68, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor of the fourth root of x (x ≥ 0) using integer arithmetic. */
static seq_t floor_fourth_root(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;
    /* Find an upper bound: keep doubling until high⁴ > x */
    for (;;) {
        seq_t sq = high * high;          /* high² */
        seq_t root = sq * sq;            /* high⁴ */
        if (root > x)
            break;
        high <<= 1;                      /* multiply by 2 */
    }
    /* Binary search between low (inclusive) and high (exclusive) */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;   /* upper mid */
        seq_t sq = mid * mid;                      /* mid² */
        seq_t root = sq * sq;                      /* mid⁴ */
        if (root <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t once for efficiency */
    seq_t sn = (seq_t)n;

    /* Compute n⁴ = (n²)² */
    seq_t sq = sn * sn;          /* n² */
    seq_t n4 = sq * sq;          /* n⁴ */

    /* Compute floor( (n⁴ / 2)^(1/4) ) and floor( (n⁴ / 4)^(1/4) ) */
    seq_t n4_div2 = n4 / 2;
    seq_t n4_div4 = n4 / 4;

    seq_t f1 = floor_fourth_root(n4_div2);
    seq_t f2 = floor_fourth_root(n4_div4);

    /* Return the sequence value */
    return sn + f1 + f2;
}
