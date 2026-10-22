/*
 * A382217  a(n) is the least k > 0 such that the factorial base expansion of
 *          n starts with that of k.
 * https://oeis.org/A382217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 302, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A382217:
 * a(n) = least k > 0 such that the factorial‑base expansion of n
 * starts with that of k.
 * Method: 1) Convert n into factorial–base digits (least‑significant first).
 *          2) Scan the digits from the most‑significant end to find the
 *             shortest prefix which is itself a valid factorial representation.
 *          3) The corresponding number is the answer.
 * Complexity: O(m²) where m ≈ log n (m ≤ 20 for 64‑bit n). */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Step 1: obtain factorial‑base digits (low→high). */
    int digits[20] = {0};          /* n < 20! for 64‑bit inputs */
    int m = 0;                     /* number of digits */
    seq_t tmp = (seq_t)n;

    while (tmp != 0 && m < 20) {
        /* digit for i! is tmp mod (i+1) where i = m+1 */
        digits[m] = (int)(tmp % ((seq_t)(m + 2)));
        tmp /= (seq_t)(m + 2);
        ++m;
    }

    /* Step 2: pre‑compute factorials up to m. */
    seq_t fact[21];
    fact[0] = 1;
    for (int i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Step 3: find the shortest valid prefix. */
    for (int t = 1; t <= m; ++t) {
        bool ok = true;
        for (int j = 1; j <= t; ++j) {
            int idx = m - t + j - 1;          /* digit index in digits[] */
            if (digits[idx] > j) {           /* factorial bound violated */
                ok = false;
                break;
            }
        }
        if (ok) {
            seq_t val = 0;
            for (int j = 1; j <= t; ++j) {
                int idx = m - t + j - 1;
                val += (seq_t)digits[idx] * fact[j];
            }
            return val;
        }
    }

    /* For completeness – this point is never reached because t = m
     * is always a valid prefix (full number).  Return n itself. */
    return (seq_t)n;
}
