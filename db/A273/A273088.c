/*
 * A273088  a(n) is a multiple of 6 and a(n)-1 or a(n)+1 is an isolated
 *          (non-twin) prime number.
 * https://oeis.org/A273088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9237 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 6 where m‑1 or m+1 is an isolated prime. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n ≥ 1 */

    /* state for memoisation */
    static seq_t nextCandidate = (seq_t)24;   /* first multiple of 6 to test */

#ifndef __cplusplus
    static seq_t *cacheC = NULL;
    static size_t szC = 0;
    static size_t capC = 0;
#else
    static std::vector<seq_t> cacheV;
#endif

    /* generate terms until we have at least n of them */
    while (
#ifndef __cplusplus
           szC < n
#else
           cacheV.size() < n
#endif
    ) {
        seq_t m = nextCandidate;
        nextCandidate += 6;          /* next multiple of 6 */

        bool good = false;

        /* check m‑1 */
        seq_t p = m - 1;
        if (isPrimeGeneric(p)) {
            if (!isPrimeGeneric(p - 2) && !isPrimeGeneric(p + 2))
                good = true;
        }

        /* if not good yet, check m+1 */
        if (!good) {
            p = m + 1;
            if (isPrimeGeneric(p)) {
                if (!isPrimeGeneric(p - 2) && !isPrimeGeneric(p + 2))
                    good = true;
            }
        }

        if (good) {
#ifdef __cplusplus
            cacheV.push_back(m);
#else
            if (szC == capC) {
                size_t new_cap = capC == 0 ? 16 : capC * 2;
                seq_t *tmp = (seq_t *)realloc(cacheC, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cacheC = tmp;
                capC = new_cap;
            }
            cacheC[szC++] = m;
#endif
        }
    }

    /* return the n‑th term (index 1‑based) */
#ifdef __cplusplus
    return cacheV[n - 1];
#else
    return cacheC[n - 1];
#endif
}
