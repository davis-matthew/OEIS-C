/*
 * A358154  a(n) is the smallest composite number obtained by appending one or
 *          more 1's to n.
 * https://oeis.org/A358154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A358154: smallest composite obtained by appending
   one or more '1's to the decimal representation of n.
   For each n it tests the three candidates
      10*n + 1,
      100*n + 11,
      1000*n + 111
   and returns the first composite.  The divisibility‑by‑3 argument
   guarantees that one of these is composite. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t cand = (seq_t)10  * sn + (seq_t)1;
    if (!isPrimeGeneric(cand))
        return cand;

    cand = (seq_t)100 * sn + (seq_t)11;
    if (!isPrimeGeneric(cand))
        return cand;

    /* the third candidate is guaranteed composite (divisible by 3) */
    return (seq_t)1000 * sn + (seq_t)111;
}
