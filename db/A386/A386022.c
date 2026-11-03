/*
 * A386022  Primes having only {0, 1, 2, 9} as digits.
 * https://oeis.org/A386022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Computes the nth prime whose decimal digits are only 0,1,2,9 by
 *            brute‑force enumeration
 * Range:     u8 n <= 10, u16 n <= 103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9874 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
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

/* Static cache for the primes found so far */
static seq_t *prime_arr = NULL;   /* array of primes */
static size_t prime_cap = 0;      /* current capacity of prime_arr   */
static size_t prime_len = 0;      /* number of stored primes         */
static size_t processed_len = 0;  /* length of numbers processed     */

/* Helper: add a prime to the cache, resizing if necessary */
static void add_prime(seq_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, prime_arr, new_cap);
        if (!tmp) abort();          /* allocation failure */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_len++] = p;
}

/* Recursively generate numbers with the given length, starting from 0 */
static void gen_rec(int pos, int len, seq_t cur)
{
    if (pos == len) {
        if (is_prime_big(cur))
            add_prime(cur);
        return;
    }

    static const int digs[4] = {0, 1, 2, 9};
    for (int i = 0; i < 4; ++i) {
        int d = digs[i];
        if (pos == 0 && d == 0) continue;   /* no leading zero */
        seq_t next = cur * 10 + d;
        gen_rec(pos + 1, len, next);
    }
}

/* Generate all primes consisting of digits {0,1,2,9} with exactly len digits */
static void generate_length(int len)
{
    gen_rec(0, len, 0);
}

/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) abort();                     /* no a(0) */

    /* Return cached value if we already have it */
    if (prime_len >= (size_t)n)
        return prime_arr[n - 1];

    /* Generate additional primes until we reach the required index */
    while (prime_len < (size_t)n) {
        ++processed_len;
        generate_length((int)processed_len);
    }

    return prime_arr[n - 1];
}
