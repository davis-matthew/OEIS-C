/*
 * A382177  a(n) is the least k > 1 such that the factorial base expansion of
 *          k*n starts with that of n while the remaining digits are zeros.
 * https://oeis.org/A382177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > 1 such that the factorial base expansion of k*n
 *            starts with that of n followed by all zeros. This is implemented
 *            by searching the shift s ≥ 1 and testing divisibility of V_s = Σ
 *            a_i·(i+s)!. a(0) is defined as 2
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 16, u64 n <= 28, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) = 2 */
    if (n == 0) return 2;

    /* 1. Extract factorial‑base digits of n.
       digits[i] corresponds to the coefficient for i! (i ≥ 1). */
    seq_t tmp = (seq_t)n;
    int digits[20] = {0};
    int m = 0;
    while (tmp > 0 && m < 20) {
        ++m;
        seq_t d = tmp % (seq_t)(m + 1);
        digits[m] = (int)d;
        tmp = tmp / (seq_t)(m + 1);
    }

    /* 2. Prepare factorials for shift s = 1.
       fact_big[i]  = (i+s)!   (exact value)
       fact_mod[i]  = fact_big[i] mod n   (value modulo n)
       Initially s = 1 => (i+1)! */
    seq_t fact_big[20] = {0};
    seq_t fact_mod[20] = {0};

    for (int i = 1; i <= m; ++i) {
        seq_t f = 1;
        for (int k = 2; k <= i + 1; ++k) f *= (seq_t)k;
        fact_big[i] = f;
        fact_mod[i] = f % (seq_t)n;
    }

    /* 3. Search for the first shift s for which V_s is divisible by n. */
    int s = 1;
    seq_t sum_mod = 0;
    for (int i = 1; i <= m; ++i) {
        sum_mod += (seq_t)digits[i] * fact_mod[i];
        sum_mod %= (seq_t)n;
    }

    while (sum_mod != 0) {
        ++s;
        for (int i = 1; i <= m; ++i) {
            seq_t mult = (seq_t)(i + s);          /* new factor for (i+s)! */
            fact_big[i] *= mult;                  /* exact factorial update */
            fact_mod[i] = (fact_mod[i] *
                           (mult % (seq_t)n)) % (seq_t)n;
        }
        sum_mod = 0;
        for (int i = 1; i <= m; ++i) {
            sum_mod += (seq_t)digits[i] * fact_mod[i];
            sum_mod %= (seq_t)n;
        }
    }

    /* 4. Compute the full V_s and divide by n to get k. */
    seq_t V = 0;
    for (int i = 1; i <= m; ++i) {
        V += (seq_t)digits[i] * fact_big[i];
    }
    seq_t k = V / (seq_t)n;
    return k;
}
