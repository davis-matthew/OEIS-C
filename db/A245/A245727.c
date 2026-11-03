/*
 * A245727  Least number k >= 0 such that n concatenated with n + k is prime.
 * https://oeis.org/A245727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that concat(n, n+k) is prime
 * Range:     u8 n <= 8, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9901 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
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

static seq_t *cache = NULL;      /* cached a(n) values (0‑based index) */
static bool *seen  = NULL;      /* true if cache[i] is valid */
static idx_t cache_cap = 0;     /* current capacity of cache tables */

/* Ensure the tables can hold indices up to (needed‑1). */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap)
        return;
    idx_t new_cap = (needed < 1024 ? 1024 : needed * 2);
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    bool *new_seen  = (bool *)realloc(seen,  new_cap * sizeof(bool));
    if (!new_cache || !new_seen)
        return;                       /* allocation failed – leave NULL */
    /* mark the newly allocated slots as unused */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_seen[i] = false;
    cache = new_cache;
    seen  = new_seen;
    cache_cap = new_cap;
}

/* Public OEIS function. */
seq_t a(idx_t n) {
    /* n is 1‑based per OEIS offset; we store at cache[n] */
    idx_t idx = n;
    ensure_capacity(idx + 1);          /* need at least n+1 slots */

    if (seen[idx])
        return cache[idx];

    /* Search for the smallest k such that concat(n, n+k) is prime. */
    for (idx_t k = 0; ; ++k) {
        idx_t candidate = n + k;
        seq_t num = concat_parts(n, candidate);
        if (is_prime_big(num)) {
            seq_t result = (seq_t)k;
            cache[idx] = result;
            seen[idx] = true;
            return result;
        }
    }
}
