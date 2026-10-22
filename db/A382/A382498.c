/*
 * A382498  Smallest k such that the fractional part of 1/k is pandigital in
 *          base n.
 * https://oeis.org/A382498
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest positive integer k such that the *
 *            fractional part of 1/k written in base n contains each digit * 0
 *            … n-1 at least once. * * The implementation searches k starting
 *            at 2, skips values * not coprime to n (since they cause a
 *            terminating expansion), * then generates the full repet...
 * Range:     u8 n <= 63, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 return zero. */
    if (n < 2)
        return 0;

    /* Search denominators starting from the smallest possible. */
    idx_t candidate = 2;         /* candidate denominator */
    while (true) {
        seq_t k = (seq_t)candidate;        /* cast to seq_t for arithmetic */

        /* Skip denominators that are not coprime to the base n,
         * because 1/k then expands terminatively and cannot
         * possibly contain all n digits. */
        if (gcd(k, (seq_t)n) != 1) {
            ++candidate;
            continue;
        }

        /* Memory for the "seen" flags; one byte per digit. */
        size_t digits = (size_t)n;
        unsigned char *seen = (unsigned char *)calloc(digits, 1);
        if (!seen) /* out of memory – unlikely in the intended tests */
            return 0;

        /* Generate the repetend of 1/k in base n. */
        uint64_t r   = 1;                          /* current remainder (mod k) */
        uint64_t kk  = (uint64_t)k;                /* denominator in 64‑bit */
        uint64_t nn  = (uint64_t)n;                /* base in 64‑bit */
        do {
            uint64_t prod   = r * nn;              /* r * base  */
            uint64_t digit  = prod / kk;          /* next digit   */
            seen[(size_t)digit] = 1;              /* mark digit   */
            r = prod % kk;                        /* new remainder */
        } while (r != 1);                         /* until cycle repeats */

        /* Verify that all digits 0 … n-1 have appeared. */
        bool ok = true;
        for (size_t i = 0; i < digits; ++i) {
            if (!seen[i]) { ok = false; break; }
        }

        free(seen);

        if (ok)            /* found the smallest k with the property */
            return k;

        ++candidate;       /* try next denominator */
    }
}
