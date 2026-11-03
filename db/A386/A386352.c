/*
 * A386352  Primes without {5, 8} as digits.
 * https://oeis.org/A386352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 2927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9450 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386352: primes that do not contain the digits 5 or 8
 * (found by a linear search, testing each integer with is_prime_big).
 * The function returns the n‑th such prime (1‑based indexing). */

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

/* Return true if x contains a decimal digit 5 or 8. */
static bool has_bad_digit(seq_t x) {
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d == (seq_t)5 || d == (seq_t)8) return true;
        x = x / (seq_t)10;
    }
    return false;
}

/* Append a seq_t value to the dynamic array 'p'.
 * The array grows by a factor of two when full. */
static void append_seq(seq_t **p, size_t *cap, size_t *len, seq_t val) {
    if (*len >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) abort();        /* unrecoverable memory failure */
        *p = tmp;
        *cap = newcap;
    }
    (*p)[(*len)++] = val;
}

/* OEIS sequence A386352. */
seq_t a(idx_t n) {
    /* Static state: array of following primes and the next integer to test. */
    static seq_t *ans = NULL;
    static size_t ans_cap = 0, ans_len = 0;
    static seq_t cur = (seq_t)2;     /* start scanning from 2 */

    /* n is 1‑based; sequences tests may not call a(0). */
    size_t target = (size_t)n;

    /* Grow the cache until we have at least 'target' applicable primes. */
    while (ans_len < target) {
        if (is_prime_big(cur)) {            /* prime test (provided by utils) */
            if (!has_bad_digit(cur)) {      /* check forbidden digits */
                append_seq(&ans, &ans_cap, &ans_len, cur);
            }
        }
        cur = cur + (seq_t)1;               /* move on to the next integer */
    }

    return ans[target - 1];
}
