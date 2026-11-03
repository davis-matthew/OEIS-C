/*
 * A262159  a(1) = 1, for n > 1 the least composite number k > a(n-1) such
 *          that a(n-1) + k is also a composite number.
 * https://oeis.org/A262159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8598 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n>1, a(n) = the smallest composite k > a(n-1)
   such that a(n-1)+k is also composite. */
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

static seq_t *cache = NULL;   /* 0‑unused, cache[1] = a(1) */
static idx_t  max_computed = 0;

static void
ensure_cache(idx_t n)
{
    if (max_computed >= n)
        return;

    size_t new_size = (size_t)n + 1;          /* indices 0 … n, 0 unused */
    seq_t *new_cache = mem_resize(seq_t, cache, new_size);
    if (!new_cache)
        abort();                        /* out‑of‑memory */
    cache = new_cache;
    cache[0] = 0;                          /* unused entry */

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {                      /* a(1) = 1 */
            cache[1] = 1;
            continue;
        }

        seq_t prev = cache[i - 1];
        seq_t k = prev + (seq_t)1;           /* start testing after prev */

        for (;;) {
            if (k > (seq_t)1 && !is_prime_big(k)) {    /* k composite */
                seq_t sum = prev + k;
                if (sum > (seq_t)1 && !is_prime_big(sum)) {
                    cache[i] = k;          /* found a(n) */
                    break;
                }
            }
            ++k;                           /* next candidate */
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* guard against underflow, offset 1 */
        return 0;
    ensure_cache(n);
    return cache[n];
}
