/*
 * A353465  Numbers k for which A276086(k) is of the form 4k+1.
 * https://oeis.org/A353465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term iff A276086(k) == 1 (mod 4), i.e. k is even
 *            and the sum of the primorial-base digits of k at primes p == 3
 *            (mod 4) is even
 * Range:     u8 n <= 65, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const unsigned A353465_PR[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

/* A276086(k) mod 4 */
static unsigned A353465_A276086_mod4(uint64_t k)
{
    unsigned r = 1;
    uint64_t rem = k;
    for (int i = 0; i < 25 && rem > 0; ++i) {
        uint64_t p = A353465_PR[i];
        uint64_t d = rem % p;
        rem /= p;
        if (d == 0) continue;
        if (p == 2) return 0;          /* even value: 2 or 0 mod 4, never 1 */
        if ((p & 3) == 3 && (d & 1)) r = (r * 3) & 3;
    }
    return r;
}

seq_t a(idx_t n)
{
    static uint64_t *found = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 0;

    while (cnt < (size_t)n) {
        uint64_t k = next_k;
        next_k += 2;
        if (A353465_A276086_mod4(k) == 1) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                found = (uint64_t *)realloc(found, cap * sizeof(uint64_t));
                if (!found) abort();
            }
            found[cnt++] = k;
        }
    }
    return (seq_t)found[(size_t)n - 1];
}
