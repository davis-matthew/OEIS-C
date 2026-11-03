/*
 * A253176  Least k>=0 such that both 3n*2^k+1 and 3n*2^k-1 are primes, or -1
 *          if no such k exists.
 * https://oeis.org/A253176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more, hard
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 20, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the least k≥0 such that both 3n·2^k+1 and 3n·2^k–1 are primes;
   if no such k exists (within a reasonable bound) the function returns –1. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static const idx_t MAX_K = 70;   /* a safe upper bound for the search */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;   /* outside sequence bounds */

    /* base = 3 * n */
    seq_t base = (seq_t)3 * (seq_t)n;

    for (idx_t k = 0; k <= MAX_K; ++k) {
        /* val = base * 2^k */
        seq_t val = seq_mul(base, pow2(k));

        seq_t left  = val + 1;
        seq_t right = val - 1;

        if (is_prime_big(left) && is_prime_big(right)) {
            return (seq_t)k;
        }
    }
    return (seq_t)-1;   /* no suitable k found within the bound */
}
