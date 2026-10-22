/*
 * A353339  Number of integers b with n > b > 1 such that the base-b
 *          representation of n is periodic.
 * https://oeis.org/A353339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353339: Number of bases b (1 < b < n) for which the base‑b
 * representation of n is periodic (i.e. a word repeated at least twice,
 * allowing leading zeros to make the representation a perfect multiple of
 * the word length).  The function implements a direct check for each
 * base b. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* No base satisfies the condition for n <= 2. */
    if (n <= 2) return 0;

    seq_t count = 0;          /* number of suitable bases */
    /* Temporary storage for the base‑b digits of n; n < 2^64, log₂ n ≤ 64. */
    uint64_t digits[64];

    for (idx_t b = 2; b < n; ++b) {
        /* Extract the minimal base‑b representation of n (least
         * significant digit first). */
        idx_t temp = n;
        int d = 0;          /* number of digits */
        while (temp > 0) {
            digits[d++] = temp % b;
            temp /= b;
        }
        if (d < 2) continue;  /* cannot be periodic with at least two repeats */

        bool periodic_found = false;

        /* Try every possible period p less than the digit length. */
        for (int p = 1; p < d && !periodic_found; ++p) {
            bool used[64] = {0};
            seq_t pat[64];
            bool ok = true;

            /* Constraints from the known digits. */
            for (int i = 0; i < d && ok; ++i) {
                int r = i % p;
                if (!used[r]) {
                    used[r] = true;
                    pat[r] = digits[i];
                } else if (pat[r] != digits[i]) {
                    ok = false;
                }
            }

            /* The representation is padded with leading zeros to length
             * L, the smallest multiple of p that is ≥ d. */
            idx_t L = ((d + p - 1) / p) * p;
            for (idx_t i = d; i < L && ok; ++i) {
                int r = i % p;
                if (!used[r]) {
                    used[r] = true;
                    pat[r] = 0;
                } else if (pat[r] != 0) {
                    ok = false;
                }
            }

            if (ok)
                periodic_found = true;
        }

        if (periodic_found)
            ++count;
    }

    return count;
}
