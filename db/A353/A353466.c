/*
 * A353466  Numbers k for which A276086(k) is of the form 4k+3.
 * https://oeis.org/A353466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term iff A276086(k) == 3 (mod 4), where
 *            A276086(k) = Product_i prime(i)^d_i, d_i the primorial-base
 *            digits of k. Mod 4: d_1 (exponent of 2) must be 0 (k even), and
 *            the number of primes p == 3 (mod 4) carrying an odd digit must
 *            be odd
 * Range:     u8 n <= 63, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static bool A353466_test(uint64_t k)
{
    static const unsigned pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
                                    53,59,61,67,71};
    uint64_t rem = k;
    unsigned res = 1;               /* product mod 4 */
    for (int i = 0; i < 20 && rem > 0; ++i) {
        uint64_t d = rem % pr[i];
        rem /= pr[i];
        if (d == 0) continue;
        if (pr[i] == 2) return false;   /* even product, never 3 mod 4 */
        if ((pr[i] & 3u) == 3u && (d & 1u)) res = (res * 3u) & 3u;
    }
    return res == 3u;
}

seq_t a(idx_t n)
{
    static uint64_t *found = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 0;

    while (cnt < (size_t)n) {
        uint64_t k = next_k;
        next_k += 2;               /* k must be even */
        if (A353466_test(k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                found = (uint64_t *)realloc(found, cap * sizeof(uint64_t));
                if (!found) abort();
            }
            found[cnt++] = k;
        }
    }
    return (seq_t)found[(size_t)n - 1];
}
