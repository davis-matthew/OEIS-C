/*
 * A287927  Numbers k such that A287925(k) is a prime.
 * https://oeis.org/A287927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k such that 16 + prime(k)^4 is prime
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-08)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_u128.h"

static idx_t *A287927_terms = NULL;
static size_t A287927_cnt = 0;
static size_t A287927_cap = 0;
static uint64_t A287927_p = 1;   /* last prime examined */
static idx_t A287927_k = 0;      /* index of last prime examined */

seq_t a(idx_t n)
{
    while (A287927_cnt < (size_t)n) {
        /* advance to next prime */
        uint64_t p = A287927_p;
        if (p < 2) p = 2;
        else if (p == 2) p = 3;
        else {
            p += 2;
            while (!is_prime_u64(p)) p += 2;
        }
        A287927_p = p;
        A287927_k++;

        /* A287925(k) = 16 + p^4, tested in 128-bit arithmetic */
        unsigned __int128 v = (unsigned __int128)p * p;
        v *= v;
        v += 16;
        if (is_prime_u128(v)) {
            if (A287927_cnt == A287927_cap) {
                A287927_cap = A287927_cap ? A287927_cap * 2 : 64;
                A287927_terms = (idx_t *)realloc(A287927_terms, A287927_cap * sizeof(idx_t));
                if (!A287927_terms) abort();
            }
            A287927_terms[A287927_cnt++] = A287927_k;
        }
    }
    return (seq_t)A287927_terms[(size_t)n - 1];
}
