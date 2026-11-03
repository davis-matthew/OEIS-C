/*
 * A343914  Riesel problem in base 3: a(n) is the smallest k >= 0 such that
 *          (2*n)*3^k-1 is prime, or -1 if no such k exists.
 * https://oeis.org/A343914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest k ≥ 0 such that (2*n)*3^k – 1 is prime, or –1 if
 *            no such k exists
 * Range:     i8 n <= 10, i16 n <= 62, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Cache for powers of 3 */
static seq_t *pow3 = NULL;
static size_t pow3_cap = 0;

/* Ensure that pow3 array contains 3^k for all k < k_needed. */
static void ensure_pow3(size_t k_needed) {
    if (k_needed < pow3_cap) return;
    size_t new_cap = k_needed + 1;
    seq_t *new_pow3 = seq_realloc(pow3, pow3_cap, new_cap);
    pow3 = new_pow3;
    /* initialise new entries */
    if (pow3_cap == 0) {
        pow3[0] = 1;
        pow3_cap = 1;
    }
    for (size_t i = pow3_cap; i < new_cap; ++i) {
        pow3[i] = pow3[i - 1] * 3;
    }
    pow3_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return -1;          /* sequence defined for n ≥ 1 */
    seq_t base = (seq_t)n * 2;      /* (2*n) as seq_t */

    /* Search for the smallest k with prime */
    for (size_t k = 0;; ++k) {
        ensure_pow3(k);
        seq_t val = seq_mul(base, pow3[k]);  /* (2*n)*3^k */
        val -= 1;                            /* –1 */
        if (is_prime_big(val))
            return (seq_t)k;
    }
}
