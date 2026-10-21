/*
 * A247047  Numbers k such that k^2 contains exactly 2 distinct digits and k^3
 *          contains exactly 3 distinct digits.
 * https://oeis.org/A247047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   For n ≥ 8: a(n) = 3 * 10^(n‑6) For n < 8: a(n) is the nth
 *            integer k such that k² has exactly 2 distinct decimal digits and
 *            k³ has exactly 3 distinct decimal digits. The first seven terms
 *            are 5, 6, 8, 9, 15, 30, 173
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   digit_counts, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* For n ≥ 8 we use the proven closed form 3 * 10^(n‑6). */
    if (n >= 8)
    {
        /* pow10 returns 10^(n-6) as seq_t; multiply by 3. */
        return 3 * pow10(n - 6);
    }

    /* Brute‑force search for the first seven terms. */
    seq_t k = 1;          /* candidate integer */
    idx_t count = 0;      /* how many terms found so far */

    while (1)
    {
        /* k² and k³ (all operations in seq_t). */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;

        unsigned char cnt[10] = {0};

        /* Count distinct digits of k². */
        digit_counts(k2, cnt);
        int distinct2 = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d])
                ++distinct2;
        if (distinct2 != 2) {
            ++k;
            continue;
        }

        /* Count distinct digits of k³. */
        digit_counts(k3, cnt);
        int distinct3 = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d])
                ++distinct3;
        if (distinct3 != 3) {
            ++k;
            continue;
        }

        /* Valid term found. */
        ++count;
        if (count == n)
            return k;
        ++k;
    }
}
