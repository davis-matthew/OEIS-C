/*
 * A382027  Primes whose decimal digits are in ascending order and also parity
 *          alternating.
 * https://oeis.org/A382027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: full, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes whose decimal digits are strictly increasing and whose parities
   alternate (odd‑even‑odd‑...). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
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

seq_t a(idx_t n)
{
    /* Cache all terms – the whole set is finite (≤511). */
    static seq_t cache[512];
    static size_t cache_len = 0;
    static bool initialized = false;

    if (!initialized) {
        for (int mask = 1; mask < (1 << 9); ++mask) {
            /* Collect the digits in ascending order. */
            int digits[9];
            int dcnt = 0;
            for (int d = 1; d <= 9; ++d) {
                if (mask & (1 << (d - 1)))
                    digits[dcnt++] = d;
            }

            /* Check parity alternation. */
            bool ok = true;
            for (int i = 1; i < dcnt; ++i) {
                if ((digits[i] & 1) == (digits[i - 1] & 1)) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                continue;

            /* Build the integer value. */
            seq_t val = 0;
            for (int i = 0; i < dcnt; ++i)
                val = val * 10 + digits[i];

            /* Keep only primes. */
            if (is_prime_big(val))
                cache[cache_len++] = val;
        }

        /* Sort the found primes. */
        mem_qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || n > cache_len)
        return (seq_t)0;

    return cache[n - 1];
}
