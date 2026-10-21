/*
 * A271676  Prime powers k such that 3k + 4 is a perfect square.
 * https://oeis.org/A271676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = prime power k such that 3*k + 4 is a perfect square. The
 *            function enumerates u with 3*k+4 = u^2, computes k, and keeps a
 *            static cache for memoization
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/pow_seq.h"

/* ----------- Cache management ------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------- Helper: integer r‑th root --------------------------------- */
static seq_t integer_root(seq_t x, idx_t r)
{
    if (r == 1) return x;
    seq_t low = 1, high = x;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t midpow = pow_seq(mid, (int)r);
        if (midpow == x) return mid;
        if (midpow < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return 0;   /* not an integer root */
}

/* ----------- Helper: test prime–power --------------------------------- */
static bool is_prime_power(seq_t k)
{
    if (k <= 1) return false;
    if (is_prime_big(k)) return true;      /* k itself is prime */
    idx_t maxExp = bitlen_seq_t(k);        /* upper bound for exponent */
    for (idx_t e = 2; e <= maxExp; ++e) {
        seq_t root = integer_root(k, e);
        if (!root) continue;
        if (pow_seq(root, (int)e) == k && is_prime_big(root))
            return true;
    }
    return false;
}

/* ----------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; guard against bad input */
    /* Persist the next u to test across calls */
    static idx_t last_u = 2;              /* start with u = 3 on first iteration */
    while (cache_len < n) {
        last_u++;                          /* next candidate u */
        seq_t u_sq = (seq_t)last_u * (seq_t)last_u;
        seq_t diff = u_sq - 4;
        if (diff % 3 == 0) {
            seq_t k = diff / 3;
            if (is_prime_power(k)) {
                cache_grow(cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - 1];
}
