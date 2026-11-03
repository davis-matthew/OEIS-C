/*
 * A386198  Primes having only {5, 6, 8, 9} as digits.
 * https://oeis.org/A386198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9997 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime whose decimal digits are all
   in the set {5,6,8,9}.  The values are computed by enumerating all
   numbers of increasing length that consist only of these digits, testing
   each for primality with the utility function `is_prime_big`, and
   collecting the primes in ascending order.  The computed primes are
   memoised across calls so that the function works correctly even
   with out‑of‑order arguments. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *primes = NULL;          // memoised primes
static size_t primes_cap = 0;         // capacity of primes array
static size_t primes_len = 0;         // current number of stored primes
static int current_len = 1;           // current decimal length being generated

// Helper to append a prime to the memoised array
static void append_prime(seq_t val) {
    if (primes_len == primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
        seq_t *new_arr = mem_resize(seq_t, primes, new_cap);
        if (!new_arr) abort();
        primes = new_arr;
        primes_cap = new_cap;
    }
    primes[primes_len++] = val;
}

// Generate all primes of a given decimal length and append them
static void generate_len(int len) {
    const seq_t DIG[4] = {5, 6, 8, 9};
    int *idx = (int *)malloc((size_t)len * sizeof(int));
    if (!idx) abort();
    for (int i = 0; i < len; ++i) idx[i] = 0;

    while (1) {
        seq_t val = 0;
        for (int p = 0; p < len; ++p) {
            val = val * 10 + DIG[idx[p]];
        }
        if (is_prime_big(val))
            append_prime(val);

        /* increment index */
        int pos;
        for (pos = len - 1; pos >= 0; --pos) {
            if (idx[pos] < 3) { idx[pos]++; break; }
            idx[pos] = 0;
        }
        if (pos < 0) break; /* all combinations exhausted */
    }
    free(idx);
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0; /* off‑by‑one guard, although n≥1 in practice */

    /* Keep generating lengths until we have at least n primes */
    while (primes_len < n) {
        generate_len(current_len);
        current_len++;
    }
    return primes[(size_t)(n - 1)];
}
