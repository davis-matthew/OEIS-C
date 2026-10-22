/*
 * A324496  Numerator of fractional part of average number of bit toggles to
 *          reach n as defined in A324495. The denominator is given in
 *          A324497.
 * https://oeis.org/A324496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the fractional part of the expected number
 *            of bit flips needed to reach the vertex n in an L‑bit hypercube,
 *            where L = floor(log2(n)) + 1. The expectation starting from
 *            distance k obeys T(0)=0, T(i)=1 + (i/L)T(i‑1) + ((L‑i)/L)T(i+1)
 *            (1 ≤ i < L), T(L)=1 + T(L‑1). Writing D(i)=T(...
 * Range:     u8 n <= 31, u16 n <= 708, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* popcount of n */
static inline int popcount_idx(idx_t n)
{
    int c = 0;
    while (n) {
        n &= n - 1;
        ++c;
    }
    return c;
}

/* main sequence function */
seq_t a(idx_t n)
{
    int L = bit_length(n);
    int k = popcount_idx(n);      /* distance from target (0 ≤ k ≤ L) */

    /* variables for current D(i) = Num/Den */
    seq_t numD = 1;
    seq_t denD = 1;               /* D(L) = 1/1 */

    /* running sum for T(k) = Σ_{i=1..k} D(i) */
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    for (int i = L; i >= 1; --i) {
        /* add D(i) to sum if i ≤ k */
        if (i <= k) {
            /* sum = sum + D(i) */
            seq_t new_num = sum_num * denD + numD * sum_den;
            seq_t new_den = sum_den * denD;
            seq_t g = gcd_seq(new_num, new_den);
            if (g > 1) {
                new_num /= g;
                new_den /= g;
            }
            sum_num = new_num;
            sum_den = new_den;
        }

        if (i == 1) break; /* next step would need D(0) which is 0 */

        /* compute D(i‑1) using recurrence
           i-1 = next, so current i is index in formula */
        seq_t num_next;
        seq_t den_next;

        /* D(i-1) = (L + (L-(i-1))·D(i)) / (i-1) */
        int next = i - 1;

        seq_t term1 = (seq_t)L * denD;
        seq_t term2 = (seq_t)(L - next) * numD;
        seq_t temp_num = term1 + term2;
        seq_t temp_den = denD * (seq_t)next;

        seq_t g = gcd_seq(temp_num, temp_den);
        if (g > 1) {
            temp_num /= g;
            temp_den /= g;
        }

        num_next = temp_num;
        den_next = temp_den;

        /* advance D to next */
        numD = num_next;
        denD = den_next;
    }

    /* result is numerator modulo denominator of T(k) */
    return sum_num % sum_den;
}
