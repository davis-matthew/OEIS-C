/*
 * A386094  Primes having only {1, 2, 4, 9} as digits.
 * https://oeis.org/A386094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this function enumerates primes whose decimal digits belong to *
 *            the set {1, 2, 4, 9}. It keeps a static cache of the sequence
 *            and * dynamically generates candidates in lexicographic (hence
 *            numeric) * order, testing each for primality with is_prime_big.
 *            If the requested * index is already in the...
 * Range:     u8 n <= 11, u16 n <= 148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9891 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms           */
    static size_t cache_cap = 0;   /* allocated capacity of cache      */

    /* Lexicographic counter over the allowed digits. */
    enum { MAX_LEN = 32 };                 /* enough for 128‑bit numbers      */
    static unsigned char idx_arr[MAX_LEN] = {0};
    static int cur_len = 1;                /* current number of digits        */

    /* The few digits permitted in the decimal representation. */
    static const seq_t DIGITS[4] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)9};

    /* If the requested term is already cached, return it. */
    if ((idx_t)n <= cache_len)
        return cache[n - 1];

    /* Number of terms we need to generate. */
    const size_t target = (size_t)n;

    /* Generate candidates until we have enough primes. */
    while (cache_len < target) {
        /* Build the current candidate from idx_arr. */
        seq_t val = 0;
        for (int i = 0; i < cur_len; ++i)
            val = val * 10 + DIGITS[idx_arr[i]];

        /* Test for primality and store if prime. */
        if (is_prime_big(val)) {
            if (cache_len == cache_cap)
                grow_seq_array(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 64);
            cache[cache_len++] = val;
        }

        /* Advance the base‑4 counter. */
        int pos = cur_len - 1;
        while (pos >= 0) {
            ++idx_arr[pos];
            if (idx_arr[pos] < 4)
                break;
            idx_arr[pos] = 0;
            --pos;
        }

        /* If all numbers of this length were exhausted, go to the next
         * length and reset the counter. */
        if (pos < 0) {
            ++cur_len;
            for (int i = 0; i < cur_len; ++i)
                idx_arr[i] = 0;
        }
    }

    /* The desired term is now cached. */
    return cache[n - 1];
}
