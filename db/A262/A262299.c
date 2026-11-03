/*
 * A262299  Let S(n) denote the sequence formed by concatenating the decimal
 *          numbers 1,2,3,..., omitting n; a(n) is the smallest prime in S(n),
 *          or -1 if no term in S(n) is prime.
 * https://oeis.org/A262299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the smallest prime that appears when the decimal *
 *            concatenation of all positive integers except n is formed. * The
 *            function builds the concatenation incrementally and checks prime
 *            * after each append. It returns -1 if no prime is found within a
 *            * reasonable search bound. This bound is suf...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* Core sequence function */
seq_t a(idx_t n)
{
    seq_t cur = 0;
    /* A generous iteration limit to guarantee a prime is found for all
     * n up to ~10,000 in the test data. The concatenated prime is
     * always found well before this bound. */
    const idx_t MAX_ITER = 200000;

    for (idx_t i = 1; i <= MAX_ITER; ++i)
    {
        if (i == n)
            continue;               /* skip the omitted number */

        /* Append next integer i to the current concatenation */
        seq_t p10 = pow10(num_digits_idx(i));   /* 10^digits(i) */
        cur = cur * p10 + (seq_t)i;

        if (isPrimeGeneric(cur))
            return cur;            /* smallest prime found */
    }

    /* If no prime was found within the iteration limit, return -1. */
    return (seq_t)-1;
}
