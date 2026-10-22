/*
 * A354861  Base-n digit sum of A002034(n)!.
 * https://oeis.org/A354861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = base‑n digit sum of A002034(n)! (Smarandache function)
 * Range:     u8 n <= 28, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   factor_u64, min_m_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/min_m_for_prime.h"

/* ------------------------------------------------------------------ */
/* Smarandache function: minimal m such that n divides m!               */

static uint64_t smarandache(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t S = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t m = min_m_for_prime(p, e);
        if (m > S)
            S = m;
    }
    return S;
}

/* ------------------------------------------------------------------ */
/* Compute a(n): base‑n digit sum of S(n)!                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2)   /* offset is 2; behaviour below does not matter */
        return 0;

    uint64_t m = smarandache(n);          /* m = S(n) */

    /* dynamic array to store the factorial in base n                           */
    size_t cap = 1;
    uint64_t *digits = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!digits) return 0;                 /* allocation failed */
    digits[0] = 1;                         /* 0! = 1 */
    size_t len = 1;

    for (idx_t x = 2; x <= m; ++x) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)x + carry;
            digits[j] = prod % (uint64_t)n;
            carry = prod / (uint64_t)n;
        }
        while (carry > 0) {
            if (len == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(digits, cap * sizeof(uint64_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = carry % (uint64_t)n;
            carry /= (uint64_t)n;
        }
    }

    /* sum the digits */
    seq_t sum = 0;
    for (size_t j = 0; j < len; ++j)
        sum += (seq_t)digits[j];

    free(digits);
    return sum;
}
