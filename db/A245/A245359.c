/*
 * A245359  Largest number k such that d_1^j + d_2^j + ... + d_r^j is prime
 *          for all j = 1..k, or 0 if no such k exists, where d_1, d_2, ...,
 *          d_r are the digits of n. a(n) = -1 if k is infinite.
 * https://oeis.org/A245359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Implements OEIS A245359:
   a(n) = largest k such that Σ d_i^j is prime for all j = 1..k,
   where d_i are the decimal digits of n.
   Returns -1 if the condition holds for all j (infinite; this
   happens exactly when n consists of at least two 1's and the
   digit count is prime), otherwise 0 if no j works, or the
   last successful j as a positive integer.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n) {
    /* Extract decimal digits of n into an array. */
    unsigned char digits[20];
    int r = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[r++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    /* All digits are '1' and at least two digits => infinite iff digit count prime. */
    if (r >= 2) {
        bool all_one = true;
        for (int i = 0; i < r; ++i) {
            if (digits[i] != 1) { all_one = false; break; }
        }
        if (all_one) {
            /* For small r we can use the generic prime test. */
            if (isPrimeGeneric((seq_t)r))
                return (seq_t)-1;   /* infinite */
            else
                return (seq_t)0;    /* fails at j=1 */
        }
    }

    /* Maintain the powers d_i^j for increasing j. */
    seq_t power[20];
    for (int i = 0; i < r; ++i) {
        power[i] = (seq_t)digits[i];   /* for j = 1 */
    }

    for (int j = 1; ; ++j) {
        /* Compute sum of current powers. */
        seq_t sum = 0;
        for (int i = 0; i < r; ++i)
            sum += power[i];

        /* Check primality. */
        if (!isPrimeGeneric(sum))
            return (seq_t)(j - 1);  /* j-1 is the maximum k that holds */

        /* Prepare powers for next j (multiply by digit again). */
        for (int i = 0; i < r; ++i)
            power[i] *= (seq_t)digits[i];
    }
}
