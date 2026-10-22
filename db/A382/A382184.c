/*
 * A382184  a(n) is the least k >= 0 such that the factorial base expansion of
 *          n starts with that of k while the remaining digits are zeros.
 * https://oeis.org/A382184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that the factorial base expansion of n *
 *            starts with that of k while the remaining digits are zeros. *
 *            Implementation uses the following observation: * * Let the
 *            factorial base digits of n (least significant first) be *
 *            d[1..m] with 0 ≤ d[i] ≤ i. For any prefix of these di...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Types seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* n == 0 is a special case. */
    if (n == 0)
        return (seq_t)0;

    /* We store at most 64 factorial digits; this is plenty for 64‑bit
     * indices (since 20! > 2^64).  For arbitrary‑precision builds the
     * loop stops naturally when n becomes zero. */
    seq_t digits[64];
    idx_t  m = 0;                 /* number of digits found so far */

    /* Compute factorial digits d[1..m] (least significant first). */
    idx_t k = 1;
    idx_t temp = n;
    while (temp > 0 && k <= 64) {
        digits[m++] = temp % (k + 1);
        temp /= (k + 1);
        ++k;
    }
    /* If k exceeded 64 we could dynamically resize, but for the
     * intended domains this will not happen. */

    /* Find the largest i0 such that the digits before i0 are all zero
     * and the remaining digits satisfy the new positional bounds.   */
    idx_t i0;
    for (i0 = m; i0 >= 1; --i0) {
        /* Check that all removed digits are zero. */
        idx_t j;
        for (j = 1; j < i0; ++j)
            if (digits[j - 1] != 0)
                break;
        if (j < i0) /* some removed digit non zero */
            continue;

        /* Check new bounds for the remaining digits. */
        idx_t i;
        for (i = i0; i <= m; ++i) {
            idx_t allowed = i - i0 + 1;          /* new positional index */
            if (digits[i - 1] > allowed)
                break;
        }
        if (i <= m) /* bound violation */
            continue;

        /* Found suitable i0. */
        break;
    }
    /* i0 should always be ≥ 1 for n > 0. */

    /* Compute the value of k from the retained digits. */
    seq_t k_val   = 0;
    seq_t fact    = 1;           /* j! */
    idx_t offset  = i0 - 1;      /* array offset of first retained digit */
    for (idx_t j = 1; j <= m - i0 + 1; ++j) {
        fact *= (seq_t)j;
        k_val += digits[offset + j - 1] * fact;
    }

    return k_val;
}
