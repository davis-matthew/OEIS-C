/*
 * A400114  Numbers whose substrings are pairwise coprime.
 * https://oeis.org/A400114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, new
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Sequence A400114: numbers whose substrings are pairwise coprime.
   The property holds only for integers with at most three decimal digits.
   The function a(n) returns the n‑th such number by brute‑force enumeration
   of 1 … 999 and testing coprimeness of all substrings. */

/* Return true if all substrings of x are pairwise coprime. */
static bool ok(seq_t x)
{
    /* Extract decimal digits (most‑significant first). */
    uint8_t d[20];
    size_t len = 0;

    if (x == 0) {
        len = 1;
        d[0] = 0;
    } else {
        seq_t t = x;
        /* Collect digits least‑significant first. */
        while (t > 0) {
            d[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
        /* Reverse to most‑significant first. */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t tmp = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = tmp;
        }
    }

    /* Generate all substrings (at most 6 for 3‑digit numbers). */
    seq_t subs[6];
    size_t k = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + d[j];
            subs[k++] = val;
        }
    }

    /* Test pairwise coprimeness of the substrings. */
    for (size_t a = 0; a < k; ++a) {
        for (size_t b = a + 1; b < k; ++b) {
            if (gcd(subs[a], subs[b]) != (seq_t)1) {
                return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t count = 0;
    for (idx_t i = 1; i <= 999; ++i) {
        if (ok((seq_t)i)) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }
    /* n exceeds the length of the admissible numbers. */
    return (seq_t)0;
}
