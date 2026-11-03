/*
 * A386154  Primes having only {2, 4, 5, 9} as digits.
 * https://oeis.org/A386154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9288 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function: a(n) generates the nth prime whose decimal digits are
    only 2, 4, 5 or 9.  The algorithm builds all numbers of a given
    length using these digits, tests each for primality with
    is_prime_big, and appends the prime to a growing static cache. */

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

/* Allowed decimal digits, in ascending order. */
static const int ALLOWED[4] = { 2, 4, 5, 9 };

/* Static cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;

/* Maximum length of numbers that have been generated so far. */
static int max_len_generated = 0;

/* Recursive generation of all numbers of a fixed length using
   ALLOWED digits, stopping when the generated number has been
   tested for primality. */
static void generate_len(int len, int pos, seq_t val)
{
    if (pos == len) {
        if (is_prime_big(val))
            append_candidate(&prime_cache, &prime_len, &prime_cap, val);
        return;
    }

    for (int i = 0; i < 4; ++i) {
        /* val * 10 + ALLOWED[i]   works for both fixed-width and
           arbitrary‑precision seq_t types. */
        generate_len(len, pos + 1, val * 10 + (seq_t)ALLOWED[i]);
    }
}

/* Return the nth prime whose digits are only {2,4,5,9}.  The offset
   is 1 (a(1) is the first such prime).  The function works for any
   order of calls and stays well under the required runtime limit. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, but guard against it */
        return 0;

    /* Generate new lengths only once.  Once a length has been
       processed, we move to the next one.                                         */
    while (prime_len < (size_t)n) {
        ++max_len_generated;
        generate_len(max_len_generated, 0, (seq_t)0);
    }

    return prime_cache[n - 1];
}
