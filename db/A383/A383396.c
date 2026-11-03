/*
 * A383396  Primes p such that p + 6, p + 10, p + 12, p + 16 and p + 22 are
 *          also primes.
 * https://oeis.org/A383396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383396 – primes p such that p+6, p+10, p+12, p+16, p+22 are also primes.
   The function performs an incremental search, memoising all found terms. */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    static idx_t   count = 0;          /* terms generated so far        */
    static seq_t *vals  = NULL;        /* dynamic array of terms        */
    static size_t  cap   = 0;          /* current capacity of vals      */
    static seq_t  cand   = 7;          /* next odd candidate to test    */

    if (n == 0) return 0;              /* undefined for 0, but safe */

    /* Initial allocation of the values array. */
    if (!vals) {
        cap = 16;
        vals = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Skip non‑prime candidates. */
        while (!isPrimeGeneric(cand)) {
            cand += (seq_t)2;          /* stay odd */
        }

        /* Test sextuple pattern. */
        if (isPrimeGeneric(cand + (seq_t)6)  &&
            isPrimeGeneric(cand + (seq_t)10) &&
            isPrimeGeneric(cand + (seq_t)12) &&
            isPrimeGeneric(cand + (seq_t)16) &&
            isPrimeGeneric(cand + (seq_t)22)) {

            /* Store the found prime. */
            if (count == cap) {
                cap <<= 1;
                vals = mem_resize(seq_t, vals, cap);
            }
            vals[count++] = cand;
        }

        cand += (seq_t)2;              /* move to next odd number     */
    }

    return vals[n - 1];
}
