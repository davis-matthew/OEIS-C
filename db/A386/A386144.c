/*
 * A386144  Primes having only {2, 3, 5, 6} as digits.
 * https://oeis.org/A386144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime * whose decimal digits are only 2,3,5,6; *
 *            all multi‑digit candidates end with 3
 * Range:     u8 n <= 7, u16 n <= 91, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9463 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static const int DIGITS[4] = {2, 3, 5, 6};

/* Recursively generate all L‑digit numbers that
 * start with the already built prefix `current`
 * and have the form: first digit + (interior digits) + 3   */
static void generate_middle(idx_t pos, idx_t L, seq_t current,
                            seq_t **arr, size_t *len, size_t *cap) {
    if (pos == L - 1) {                    /* last position: append final 3 */
        seq_t val = current * 10 + 3;
        if (is_prime_big(val))
            append_candidate(arr, len, cap, val);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        generate_middle(pos + 1, L,
                        current * 10 + DIGITS[i],
                        arr, len, cap);
    }
}

/* Generate all L‑digit numbers with the required digit set
 * and test them for primality. */
static void generate_length(idx_t L, seq_t **arr, size_t *len,
                            size_t *cap) {
    if (L == 1) {                          /* single‑digit candidates */
        for (int i = 0; i < 4; ++i) {
            seq_t val = DIGITS[i];
            if (is_prime_big(val))
                append_candidate(arr, len, cap, val);
        }
    } else {                                /* multi‑digit candidates */
        for (int i = 0; i < 4; ++i) {
            seq_t first = DIGITS[i];
            generate_middle(1, L, first, arr, len, cap);
        }
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t generated_len = 0;         /* maximum length generated so far */

    while (cache_len < n) {
        ++generated_len;                     /* increase length */
        generate_length(generated_len,
                        &cache, &cache_len, &cache_cap);
    }
    return cache[n - 1];
}
