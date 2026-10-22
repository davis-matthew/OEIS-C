/*
 * A322476  Primes that are not base-11 deletable primes (written in base 10).
 * https://oeis.org/A322476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 890, u32 n <= 890, u64 n <= 890,
 *            u128 n <= 890, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is NOT a base‑11 deletable prime (OEIS A322476)
   by ordinary sieve search and recursive deletability check. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

typedef struct {
    seq_t key;
    bool value;
} deletion_cache_item;

static deletion_cache_item *deletion_cache = NULL;
static size_t deletion_cache_len   = 0;
static size_t deletion_cache_cap   = 0;

/* Ensure the cache has space for at least `need` items. */
static void ensure_cache_capacity(size_t need) {
    if (deletion_cache_cap >= need) return;
    size_t new_cap = deletion_cache_cap ? deletion_cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    deletion_cache = (deletion_cache_item *)realloc(deletion_cache,
                                                    new_cap * sizeof(deletion_cache_item));
    if (!deletion_cache) abort();                /* memory allocation failure */
    deletion_cache_cap = new_cap;
}

/* Search for a key in the cache.  Return true if found and store the value in out. */
static bool cache_lookup(seq_t key, bool *out) {
    for (size_t i = 0; i < deletion_cache_len; ++i) {
        if (deletion_cache[i].key == key) {
            *out = deletion_cache[i].value;
            return true;
        }
    }
    return false;
}

/* Insert a new key–value pair into the cache. */
static void cache_insert(seq_t key, bool value) {
    ensure_cache_capacity(deletion_cache_len + 1);
    deletion_cache[deletion_cache_len].key   = key;
    deletion_cache[deletion_cache_len].value = value;
    deletion_cache_len++;
}

/* Recursively determine whether `p` is a base‑11 deletable prime.  */
static bool is_deletable_prime(seq_t p) {
    bool cached;
    if (cache_lookup(p, &cached))
        return cached;

    /* Convert `p` to base‑11 digits, most significant digit first. */
    seq_t tmp = p;
    uint8_t digits[64];  /* enough for numbers up to 2^256 in base 11 */
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 11);
        tmp /= 11;
    }
    if (len == 0) {       /* p == 0, unlikely but safe */
        cache_insert(p, false);
        return false;
    }
    /* Reverse to most‑significant first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    bool result = false;
    if (len == 1) {
        /* Single‑digit prime is deletable by definition. */
        result = true;
    } else {
        /* Try all deletions that do not introduce a leading zero. */
        for (size_t i = 0; i < len; ++i) {
            if (i == 0 && len > 1 && digits[1] == 0) continue;  /* leading zero would appear */
            seq_t sub = 0;
            for (size_t j = 0; j < len; ++j) {
                if (j == i) continue;
                sub = sub * 11 + (seq_t)digits[j];
            }
            if (is_prime_big(sub) && is_deletable_prime(sub)) {
                result = true;
                break;
            }
        }
    }
    cache_insert(p, result);
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Maintain a list of non‑deletable primes found so far. */
    static seq_t *non_del = NULL;
    static size_t non_del_len = 0;
    static size_t non_del_cap = 0;

    /* Iterator for generating primes. */
    static seq_t current = 2;

    /* Ensure the non‑deletable list contains at least n elements. */
    while (non_del_len < n) {
        seq_t cand = current;

        /* Find the next prime. */
        while (true) {
            if (is_prime_big(cand)) {
                if (!is_deletable_prime(cand)) {
                    /* Add to list */
                    if (non_del_cap == non_del_len) {
                        size_t new_cap = non_del_cap ? non_del_cap * 2 : 16;
                        non_del = (seq_t *)realloc(non_del,
                                                   new_cap * sizeof(seq_t));
                        if (!non_del) abort(); /* allocation failure */
                        non_del_cap = new_cap;
                    }
                    non_del[non_del_len++] = cand;
                }
                /* Move to the next odd candidate. */
                current = cand + ((cand == 2) ? 1 : 2);
                break;
            }
            cand += ((cand == 2) ? 1 : 2);
        }
    }

    return non_del[n - 1];
}
