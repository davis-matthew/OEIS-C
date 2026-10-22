/*
 * A317252  (Number of 4 X 4 pandiagonal magic squares with distinct positive
 *          entries less than n)/384.
 * https://oeis.org/A317252
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the series expansion of
 *            x^17*(1+x)*(1+2x+6x^2+8x^3+17x^4+20x^5+36x^6+38x^7+58x^8+57x^9+76x^10+
 *            68x^11+84x^12+70x^13+81x^14+57x^15+59x^16+34x^17+38x^18+16x^19+14x^20)
 *            divided by (1−x^3)(1−x^4)(1−x^6)(1−x^7)(1−x^8)(1−x^10) . The
 *            sequence starts at a(17)=1. The func...
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External linkage, as required. */
seq_t a(idx_t n)
{
    if (n < 17)                      /* defined only for n ≥ 17 */
        return 0;

    /* The first 22 non‑zero terms (coefficients of the numerator). */
    static const seq_t init[22] = {
        1,  3,   8,  15,  29,  48,   80,  121, 182, 260,
        367, 497, 673, 886, 1158, 1477, 1875, 2341,
        2913, 3570, 4355, 5258
    };

    /* Degrees of the factors in the denominator (1−x^k).  The product
       has total degree 38. */
    static const int ks[6] = {3, 4, 6, 7, 8, 10};

    /* Coefficients of Q(x)=∏(1−x^k).  Computed once. */
    static bool coeff_done = false;
    static int32_t coeff[39];   /* 0 .. 38 */
    if (!coeff_done) {
        for (int i = 0; i <= 38; ++i) coeff[i] = 0;
        coeff[0] = 1;
        int deg = 0;
        for (int k_idx = 0; k_idx < 6; ++k_idx) {
            int k = ks[k_idx];
            for (int j = deg + k; j >= k; --j)
                coeff[j] = coeff[j] - coeff[j - k];
            deg += k;
        }
        coeff_done = true;
    }

    /* For n ≤ 38 we can return the initial terms directly. */
    if (n <= 38) {
        /* 17 ≤ n ≤ 38 → init[n−17] is defined. */
        return init[n - 17];
    }

    /* Ring buffer of the last 38 computed terms (a(k) stored at index k%38). */
    seq_t buf[38] = {0};

    /* Iterate from 1 up to n, computing each term and storing it. */
    for (idx_t cur = 1; cur <= n; ++cur) {
        seq_t val;
        if (cur <= 38) {                   /* 17 ≤ cur ≤ 38 */
            if (cur >= 17)
                val = init[cur - 17];
            else
                val = 0;
        } else {                            /* cur ≥ 39 : use recurrence */
            seq_t sum_pos = 0, sum_neg = 0;
            for (int d = 1; d <= 38; ++d) {
                int32_t c = coeff[d];
                if (c == 0) continue;
                seq_t term = buf[(cur - d) % 38];
                if (c > 0)
                    sum_pos += term * (seq_t)c;
                else
                    sum_neg += term * (seq_t)(-c);
            }
            val = sum_neg - sum_pos;        /* a(cur) = Σ_{c<0}(-c)a(cur−d) − Σ_{c>0}c a(cur−d) */
        }

        buf[cur % 38] = val;

        if (cur == n)
            return val;
    }

    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
