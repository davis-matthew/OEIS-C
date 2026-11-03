/*
 * A327823  Odd integers m such that every odd integer k with 1 < k < m and
 *          gcd(k,m) = 1 is prime.
 * https://oeis.org/A327823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property sequence: a(n) = nth odd integer m such that every odd k with 1<k<m
   and gcd(k,m)=1 is prime. This sequence ends at m = 105. */

/* The sequence is computed by brute‑force search up to the known upper bound 105.
   No auxiliary data structures are required. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd.h"
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

static seq_t _seq[128];
static idx_t _seq_len = 0;

/* Precompute all terms up to the known maximum (105).  */
static void _precompute(void) {
    if (_seq_len != 0) return;          /* already done */
    for (idx_t m = 1; m <= 105; m += 2) {   /* only odd m */
        bool ok = true;
        for (idx_t k = 3; k < m; k += 2) {   /* odd k 1<k<m */
            seq_t g = gcd((seq_t)m, (seq_t)k);
            if (g == 1 && !isPrimeGeneric((seq_t)k)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            _seq[_seq_len++] = (seq_t)m;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* undefined for n=0 or negative */
    _precompute();
    if (n-1 < _seq_len) {
        return _seq[n-1];
    } else {
        /* sequence has no further terms beyond 105 */
        return 0;
    }
}
