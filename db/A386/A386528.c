/*
 * A386528  Primes which remain primes after the mapping {1 -> 3, 3 -> 5, 5 ->
 *          7, 7 -> 9, 9 -> 1} of its decimal digits.
 * https://oeis.org/A386528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = p where p is the n-th prime that remains prime after the
 *            digit mapping 1→3, 3→5, 5→7, 7→9, 9→1. The algorithm generates
 *            primes sequentially until it has found n such special primes,
 *            caching the results for subsequent calls
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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

/* Transform a prime according to the mapping */
static seq_t map_prime(seq_t n)
{
    /* up to 39 decimal digits for a 128‑bit number, 80 bytes is safe */
    uint8_t digits[80];
    size_t len = 0;
    seq_t tmp = n;
    if (tmp == 0) return 0;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    seq_t res = 0;
    for (size_t i = len; i-- > 0;) {
        uint8_t d = digits[i];
        uint8_t m;
        switch (d) {
            case 1: m = 3; break;
            case 3: m = 5; break;
            case 5: m = 7; break;
            case 7: m = 9; break;
            case 9: m = 1; break;
            default: m = d; break;
        }
        res = res * 10 + m;
    }
    return res;
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    /* Cached terms */
    static seq_t *cache = NULL;      /* array of found primes */
    static size_t cap = 0;           /* allocated capacity */
    static size_t sz = 0;            /* number of stored terms */
    static seq_t last_checked = 1;   /* last candidate examined */

    /* If we already have the n‑th term cached, return it. */
    if (sz >= (size_t)n) {
        return cache[n - 1];
    }

    /* Search for further terms until we have at least n of them. */
    for (seq_t cand = last_checked + 1; ; ++cand) {
        if (!is_prime_big(cand)) continue;          /* not prime */
        seq_t mapped = map_prime(cand);
        if (!is_prime_big(mapped)) continue;        /* mapped not prime */

        /* Ensure cache has room. */
        if (sz == cap) {
            size_t new_cap = cap ? cap * 2 : 128;
            grow_seq(&cache, &cap, new_cap);
        }
        cache[sz++] = cand;
        last_checked = cand;

        if (sz >= (size_t)n) break;
    }

    return cache[n - 1];
}
