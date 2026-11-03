/*
 * A255283  Number of odd terms in f^n, where f = (1/x+1+x)*(1/y+1+y)-x-y.
 * https://oeis.org/A255283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RunLengthTransform(b) * * b(1)=7, b(2)=31, b(3)=145,
 *            b(4)=601, * for k≥5 : b(k)=4*b(k‑1)+147 * * a(0)=1, and for n>0
 *            a(n) is the product of b(k) * over the lengths k of consecutive
 *            1‑runs in the binary * expansion of n
 * Range:     u8 n <= 14, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t get_b(idx_t k)
{
    /* cache computed values up to k=64 */
    static seq_t cache[65];
    static bool have[65] = {0};

    /* base cases */
    if (k == 1) { cache[1] = (seq_t)7;   have[1] = true; }
    else if (k == 2) { cache[2] = (seq_t)31; have[2] = true; }
    else if (k == 3) { cache[3] = (seq_t)145; have[3] = true; }
    else if (k == 4) { cache[4] = (seq_t)601; have[4] = true; }
    else if (k >= 5)
    {
        /* Ensure values up to k are computed */
        /* start from the last known value (k=4) */
        seq_t val = cache[4];
        for (idx_t i = 5; i <= k; ++i)
        {
            val = val * (seq_t)4 + (seq_t)147;
            cache[i] = val;
            have[i] = true;
        }
    }

    return cache[k];
}

extern seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    idx_t temp = n;

    while (temp)
    {
        /* Skip trailing zeros in the binary representation */
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            if (temp == 0)
                break;
        }
        if (temp == 0)
            break;

        /* Count a maximal run of consecutive 1s */
        idx_t run = 0;
        while ((temp & 1) == 1)
        {
            ++run;
            temp >>= 1;
        }

        prod *= get_b(run);
    }

    return prod;
}
