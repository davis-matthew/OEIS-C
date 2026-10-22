/*
 * A394749  Largest GCD of the digits of n in any base b < n, considering only
 *          cases where all digits are nonzero.
 * https://oeis.org/A394749
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4026, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A394749: For each n (>=2), find the maximum gcd of the digits of n when written
 * in any base b with 2 <= b < n, provided all digits are non‑zero.
 * The function iterates over all admissible bases, checks that no digit is zero,
 * computes the gcd of the digits, and keeps the largest such gcd.
 * If no base produces all non‑zero digits (only possible for n=2), 0 is returned. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle offset: n < 2 irrelevant, n=2 has no valid base */
    if (n < 3) return (seq_t)0;

    seq_t best = (seq_t)0;

    for (idx_t b = 2; b < n; ++b) {
        idx_t tmp = n;
        seq_t g = (seq_t)0;
        bool ok = true;

        /* Scan digits of n in base b */
        while (tmp > 0) {
            idx_t digit = tmp % b;
            if (digit == 0) { /* zero digit -> skip this base */
                ok = false;
                break;
            }
            g = gcd(g, (seq_t)digit);
            tmp /= b;
        }

        if (ok && g > best) best = g;
    }

    return best;
}
