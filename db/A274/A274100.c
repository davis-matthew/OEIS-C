/*
 * A274100  Number of partitions of 2^n into at most four parts.
 * https://oeis.org/A274100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274100 – Number of partitions of 2^n into at most four parts.
   Formula: a(n) = p₄(2^n), where p₄(N) is the number of partitions of N
   into parts of size ≤ 4.  It can be written as a quasi‑polynomial
   (degree 3, period 12).  The coefficients for each residue class
   modulo 12 are pre‑computed once and reused for all calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t  base_coeff[12];
static long long first_coeff[12];
static long long second_coeff[12];
static long long third_coeff[12];
static bool initialized = false;

static void init_tables(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Compute p₄(N) for N = 0 … 60; this suffices to determine the
       quasi‑polynomial coefficients. */
    seq_t dp[61];
    for (size_t i = 0; i <= 60; ++i)
        dp[i] = 0;
    dp[0] = 1;
    for (int coin = 1; coin <= 4; ++coin) {
        for (int n = coin; n <= 60; ++n) {
            dp[n] += dp[n - coin];
        }
    }

    /* Pre‑compute the coefficients for each residue r mod 12. */
    for (int r = 0; r < 12; ++r) {
        seq_t f0 = dp[r];
        seq_t f1 = dp[r + 12];
        seq_t f2 = dp[r + 24];
        seq_t f3 = dp[r + 36];

        base_coeff[r]  = f0;
        first_coeff[r] = (long long)f1 - (long long)f0;
        second_coeff[r] = (long long)f2 - 2LL * (long long)f1 + (long long)f0;
        third_coeff[r]  = (long long)f3 - 3LL * (long long)f2
                          + 3LL * (long long)f1 - (long long)f0;
    }
}

seq_t a(idx_t n)
{
    init_tables();

    /* N = 2^n.  pow2 returns a seq_t value. */
    seq_t N = pow2(n);

    /* r = N mod 12, k = N / 12. */
    seq_t twelve = (seq_t)12;
    idx_t   r = (idx_t)(N % twelve);
    seq_t   k = N / twelve;

    seq_t result = base_coeff[r];

    /* Linear term: first_coeff[r] * k. */
    if (first_coeff[r] >= 0)
        result += (seq_t)first_coeff[r] * k;
    else
        result -= (seq_t)(-first_coeff[r]) * k;

    /* Quadratic term: second_coeff[r] * k*(k-1)/2. */
    seq_t one = (seq_t)1;
    if (k >= one) {
        seq_t comb2 = k * (k - one) / (seq_t)2;
        if (second_coeff[r] >= 0)
            result += (seq_t)second_coeff[r] * comb2;
        else
            result -= (seq_t)(-second_coeff[r]) * comb2;
    }

    /* Cubic term: third_coeff[r] * k*(k-1)*(k-2)/6. */
    seq_t two = (seq_t)2;
    if (k >= two) {
        seq_t comb3 = k * (k - one) * (k - two) / (seq_t)6;
        if (third_coeff[r] >= 0)
            result += (seq_t)third_coeff[r] * comb3;
        else
            result -= (seq_t)(-third_coeff[r]) * comb3;
    }

    return result;
}
