/*
 * A300016  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 2.
 * https://oeis.org/A300016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1087, u32 n <= 9759, u64 n <= 9759,
 *            u128 n <= 9759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(1)=2. For n>1, a(n) is the smallest unused integer m such that
   the cumulative sum S = Σ_{i=1}^n a(i) contains at least one digit 2. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---- global state ---- */
static seq_t *cache      = NULL;   /* sequence values          */
static size_t cache_len  = 0;      /* number of entries stored */
static size_t cache_cap  = 0;      /* capacity of cache array  */

static uint8_t *used      = NULL;  /* 1 if number has been used */
static size_t used_cap    = 0;      /* capacity of used array    */

/* ---- helpers ---- */
static bool has_digit2(seq_t x)
{
    if (x == 0) return false;
    while (x > 0) {
        if (x % 10 == 2) return true;
        x /= 10;
    }
    return false;
}

static void ensure_used_capacity(idx_t needed)
{
    if (needed <= used_cap) return;
    size_t old = used_cap;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    if (new_cap < (size_t)needed) new_cap = needed;
    uint8_t *new_arr = (uint8_t *)realloc(used, new_cap);
    if (!new_arr) abort();               /* allocation failure */
    used = new_arr;
    memset(used + old, 0, new_cap - old);
    used_cap = new_cap;
}

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t old = cache_cap;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();               /* allocation failure */
    cache = new_arr;
    /* newly allocated slots will be written before use */
    cache_cap = new_cap;
}

/* ---- public function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, safe fallback */

    /* return cached value if already known */
    if (n <= cache_len) return cache[n - 1];

    /* static cumulative sum of the already generated terms */
    static seq_t cumulative_sum = 0;

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* search for the smallest unused integer that keeps a digit 2 in the sum */
        for (idx_t cand = 1; ; ++cand) {
            ensure_used_capacity(cand + 1);
            if (used[(size_t)cand]) continue;              /* already used */

            seq_t new_sum = cumulative_sum + (seq_t)cand;
            if (has_digit2(new_sum)) {
                /* accept cand as a(k) */
                ensure_cache_capacity(cache_len + 1);
                cache[cache_len] = (seq_t)cand;
                ++cache_len;
                used[(size_t)cand] = 1;
                cumulative_sum = new_sum;
                break;
            }
        }
    }

    return cache[n - 1];
}
