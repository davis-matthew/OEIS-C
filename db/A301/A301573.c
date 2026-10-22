/*
 * A301573  Distance to nearest perfect power n^k, k>=2 (A001597).
 * https://oeis.org/A301573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{k>=2} |n - m^k| for integer m>=1, computed by
 *            enumerating k and searching for the floor root of n. For each
 *            exponent k we also test the next integer base
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bitlen_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/bitlen_seq.h"

/* Absolute difference between two seq_t values. */
static seq_t seq_abs(seq_t a, seq_t b)
{
    return (a >= b) ? a - b : b - a;
}

/* Integer floor k‑th root of n: largest b such that b^k <= n.
   Uses binary search and big_pow. */
static seq_t kth_root_floor(seq_t n, idx_t k)
{
    seq_t low = 1;
    seq_t high = n;
    seq_t ans = 1;
    while (low <= high)
    {
        seq_t mid = (low + high) / 2;
        seq_t val = big_pow(mid, (seq_t)k);
        if (val <= n)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 0 explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;

    /* Start with distance to 1 (the smallest perfect power). */
    seq_t best = n_seq - 1;

    /* Exponents larger than the bit length of n cannot
       produce a base > 1. Stop when base becomes 1. */
    idx_t kmax = (idx_t)bitlen_seq(n_seq) + 1;

    for (idx_t k = 2; k <= kmax; ++k)
    {
        seq_t base = kth_root_floor(n_seq, k);

        /* Perfect power for the floor root. */
        seq_t pow_floor = big_pow(base, (seq_t)k);
        seq_t diff1 = seq_abs(n_seq, pow_floor);
        if (diff1 == 0)
            return (seq_t)0;
        if (diff1 < best)
            best = diff1;

        /* Check the next possible base (base+1). */
        seq_t pow_ceiling = big_pow(base + (seq_t)1, (seq_t)k);
        seq_t diff2 = seq_abs(n_seq, pow_ceiling);
        if (diff2 < best)
            best = diff2;

        /* If the floor root becomes 1, further exponents
           cannot improve the distance. */
        if (base == 1)
            break;
    }

    return best;
}
