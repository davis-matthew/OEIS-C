/*
 * A397308  Positive numbers m such that m - 2^k is a prime for exactly two
 *          values of k > 0.
 * https://oeis.org/A397308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer m > 0 such that m‑2^k is a prime for
 *            exactly two values of k > 0. In other words, m has exactly two
 *            representations of the form m = p + 2^k with p prime
 * Range:     u8 n <= 37, u16 n <= 7708, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9614 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/pow2.h"
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

static seq_t *terms   = NULL;  /* Cached sequence values                */
static size_t terms_len = 0;   /* Number of terms currently cached      */
static size_t terms_cap = 0;   /* Capacity of the cached array          */
static seq_t next_candidate = 5; /* Last odd number checked (start before 7) */

seq_t a(idx_t n)
{
    /* Compute terms up to index n, memoizing results */
    while (terms_len < (size_t)n) {
        /* Move to the next odd candidate */
        next_candidate += 2;          /* odd numbers only */
        seq_t m = next_candidate;

        /* Count representations m = p + 2^k, k > 0, p prime */
        size_t count = 0;
        seq_t max_pow = m - 2; /* largest 2^k that can be subtracted from m */
        seq_t pow2k   = 2;     /* 2^1 */

        while (pow2k <= max_pow) {
            if (isPrimeGeneric(m - pow2k)) {
                if (++count > 2) break; /* early exit */
            }
            /* Safeguard against overflow of pow2k << 1 */
            if (pow2k > max_pow / 2) break;
            pow2k <<= 1;
        }

        if (count == 2) {
            /* Grow storage if necessary */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = m;
        }
    }

    return terms[(size_t)(n - 1)];
}
