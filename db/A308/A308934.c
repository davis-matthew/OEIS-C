/*
 * A308934  Number of ways to write n as (2^a*3^b)^2 + (2^c*3^d)^2 + x^2 +
 *          2*y^2, where a,b,c,d,x,y are nonnegative integers with 2^a*3^b >=
 *          2^c*3^d.
 * https://oeis.org/A308934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as (2^a·3^b)^2 +
 *            (2^c·3^d)^2 + x^2 + 2·y^2 with a,b,c,d,x,y ≥ 0 and 2^a·3^b ≥
 *            2^c·3^d
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/floor_sqrt.h"

/* Count all quadruples (a,b,c,d,x,y) satisfying the conditions. */
seq_t a(idx_t n)
{
    /* a(1) is defined for index 1; indices <1 are undefined. */
    if (n < 1)
        return 0;

    /* largest possible value of 2^a·3^b that can appear */
    seq_t maxS = floor_sqrt((seq_t)n);

    /* collect all 2^a·3^b <= maxS in a small array */
    seq_t svals[1024];
    size_t scnt = 0;

    for (idx_t aExp = 0;
         /* can safely compute 2^aExp because 2^aExp <= maxS <= sqrt(n) */
         (seq_t)pow2(aExp) <= maxS;
         ++aExp) {
        seq_t p2 = pow2(aExp);
        for (idx_t bExp = 0;
             (seq_t)pow3(bExp) <= maxS;
             ++bExp) {
            seq_t p3 = pow3(bExp);
            seq_t val = p2 * p3;
            if (val > maxS)
                break;  /* further bExp will only increase val */
            svals[scnt++] = val;
        }
    }

    seq_t count = 0;

    /* iterate over all pairs (i,j) with s[i] >= s[j] */
    for (size_t i = 0; i < scnt; ++i) {
        seq_t sq1 = svals[i] * svals[i];
        for (size_t j = 0; j <= i; ++j) {
            seq_t sq2 = svals[j] * svals[j];
            seq_t sumsq = sq1 + sq2;
            if (sumsq > (seq_t)n)
                continue;  /* this pair is too large */
            seq_t rem = (seq_t)n - sumsq;   /* remaining x^2 + 2*y^2 */

            /* y ranges from 0 to floor( sqrt(rem/2) ) */
            seq_t half = rem / 2;
            seq_t sqrt_half = floor_sqrt(half);
            idx_t y_max = (idx_t)sqrt_half;

            for (idx_t y = 0; y <= y_max; ++y) {
                seq_t sub = (seq_t)2 * (seq_t)y * (seq_t)y;
                seq_t x_sq = rem - sub;
                seq_t x = floor_sqrt(x_sq);
                if (x * x == x_sq)
                    ++count;
            }
        }
    }

    return count;
}
