/*
 * A265544  a(n) = smallest base-3 palindrome m >= n such that every base-3
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑3 palindrome m ≥ n such that for each
 *            position, the base‑3 digit of n is ≤ the corresponding digit of
 *            m. The function searches upward from n, testing each candidate
 *            for the digitwise dominance condition and for base‑3
 *            palindromicity
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* --- digits of n (least‑significant first) ---
       n is an idx_t (uint64_t), so its base‑3 digits fit in a small buffer. */
    uint64_t nn = (uint64_t)n;      /* normal integer copy of n */
    uint8_t nd[256];
    size_t ndlen = 0;
    if (nn == 0) {
        nd[0] = 0;
        ndlen = 1;
    } else {
        while (nn > 0) {
            nd[ndlen++] = (uint8_t)(nn % 3);
            nn /= 3;
        }
    }

    /* --- search for the minimal m satisfying the conditions --- */
    for (seq_t m = n; ; ++m) {
        /* digits of m */
        seq_t t = m;
        uint8_t md[256];
        size_t mdlen = 0;
        if (t == 0) {
            md[0] = 0;
            mdlen = 1;
        } else {
            while (t > 0) {
                /* t % 3 is a seq_t; cast to unsigned to store in md[] */
                md[mdlen++] = (unsigned)(t % 3);
                t /= 3;
            }
        }

        /* digitwise dominance check: each digit of n <= corresponding digit of m */
        bool ok = true;
        for (size_t i = 0; i < ndlen; ++i) {
            if (md[i] < nd[i]) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            continue;
        }

        /* palindromicity check in base 3 */
        if (!is_palindrome(m, 3)) {
            continue;
        }

        return m;
    }
}
