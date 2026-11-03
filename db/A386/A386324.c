/*
 * A386324  Primes without {0, 6} as digits.
 * https://oeis.org/A386324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 3200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9618 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-Formula: a(n) is the n‑th prime that does not contain the digits 0 or 6. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static inline bool has_zero_or_six(seq_t x)
{
    for (; x > 0; x /= 10)
    {
        seq_t d = x % 10;
        if (d == 0 || d == 6)
            return true;
    }
    return false;
}

static seq_t next_valid_prime(void)
{
    static seq_t cand = 2;
    for (;;)
    {
        if (cand == 2)
        {
            if (isPrimeGeneric(cand) && !has_zero_or_six(cand))
            {
                cand = 3;
                return 2;
            }
            cand = 3;
            continue;
        }

        if (isPrimeGeneric(cand) && !has_zero_or_six(cand))
        {
            seq_t result = cand;
            cand += 2;
            return result;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    static seq_t *values = NULL;
    static size_t capacity = 0;
    static size_t length = 0;

    while (length < n)
    {
        seq_t next = next_valid_prime();
        if (length == capacity)
        {
            capacity = capacity ? capacity * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, values, capacity);
            values = tmp;
        }
        values[length++] = next;
    }
    return values[n - 1];
}
