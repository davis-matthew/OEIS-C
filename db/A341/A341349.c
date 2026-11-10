/*
 * A341349  Numbers k for which A254049(k) is not a multiple of 3.
 * https://oeis.org/A341349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A254049(k) mod 3 != 0, where A254049(k)
 *            = (P+1)/2 and P = A003961(2k-1); A254049(k) is divisible by 3
 *            iff P == 2 (mod 3). Computed by a scan over k
 * Range:     u8 n <= 125, u16 n <= 16369, u32 n <= 16369, u64 n <= 16369,
 *            u128 n <= 16369, big any n
 * Validated: standard, pass, 90 terms through n = 9910 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t k = 0;

    while (cnt < (size_t)n) {
        k++;
        uint64_t m = 2 * k - 1;
        uint64_t pr[64], ex[64];
        size_t c = 0;
        uint64_t P = 1; /* product mod 3 */
        if (m > 1) {
            factor_u64(m, pr, ex, &c);
            for (size_t i = 0; i < c; ++i) {
                uint64_t q = next_prime(pr[i]) % 3;
                for (uint64_t j = 0; j < ex[i]; ++j)
                    P = (P * q) % 3;
            }
        }
        if (P != 2) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
