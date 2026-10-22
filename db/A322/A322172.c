/*
 * A322172  Primes that are not base-7 deletable primes (written in base 10).
 * https://oeis.org/A322172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 5287, u32 n <= 5894, u64 n <= 9626,
 *            u128 n <= 9626, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the nth prime that is NOT base‑7‑deletable.
   This is the sequence A322172. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"

/* Memoisation of the deletable property for decimal numbers. */
static int8_t *del_cache      = NULL;   /* -1 unknown, 0 false, 1 true */
static size_t del_cache_size  = 0;

static void
ensure_del_cache_size(size_t sz)
{
    if (sz <= del_cache_size) return;
    size_t new_size = del_cache_size ? del_cache_size << 1 : 256;
    while (new_size < sz) new_size <<= 1;
    int8_t *p = (int8_t *)realloc(del_cache, new_size * sizeof(int8_t));
    if (!p) { /* allocation failed – use the old pointer */ return; }
    memset(p + del_cache_size, -1, (new_size - del_cache_size) * sizeof(int8_t));
    del_cache = p;
    del_cache_size = new_size;
}

/* Recursive check whether a decimal number n is a base‑7 deletable prime. */
static bool
deletable_rec(uint64_t n)
{
    if (n < 7) return true;          /* single‑digit primes are deletable */

    ensure_del_cache_size(n + 1);
    if (del_cache[n] != -1) return del_cache[n];

    /* Get base‑7 digits (big‑endian). */
    uint64_t tmp = n;
    int len = 0;
    while (tmp) { tmp /= 7; ++len; }
    int digits[20];
    tmp = n;
    for (int i = len - 1; i >= 0; --i) {
        digits[i] = tmp % 7;
        tmp /= 7;
    }

    /* Try deleting each digit. */
    for (int j = 0; j < len; ++j) {
        /* Deletion must not create a leading zero. */
        if (j == 0 && len > 1 && digits[1] == 0) continue;

        uint64_t y = 0;
        for (int k = 0; k < len; ++k) {
            if (k == j) continue;
            y = y * 7 + digits[k];
        }
        if (y == 0) continue;          /* empty string handled by n<7 */

        if (is_prime_big((seq_t)y) && deletable_rec(y)) {
            del_cache[n] = 1;
            return true;
        }
    }
    del_cache[n] = 0;
    return false;
}

/* Cache of computed sequence values a(1), a(2), ...  */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

static void
ensure_seq_capacity(size_t sz)
{
    if (sz <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < sz) new_cap = sz;
    seq_t *p = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!p) { /* allocation failed – use the old pointer */ return; }
    seq_cache = p;
    seq_cap   = new_cap;
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    static uint64_t candidate = 2;            /* next integer to test */

    while (seq_len < n) {
        /* Find the next prime. */
        uint64_t prime;
        if (candidate == 2) {
            prime = 2;
            candidate = 3;
        } else {
            for (;;) {
                if (is_prime_big((seq_t)candidate)) break;
                candidate += 2;
            }
            prime = candidate;
            candidate += 2;
        }

        /* Check if prime is NOT base‑7 deletable. */
        if (!deletable_rec(prime)) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)prime;
        }
    }
    return seq_cache[n - 1];
}
