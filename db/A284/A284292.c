/*
 * A284292  Primes containing a digit 8.
 * https://oeis.org/A284292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 8346 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence of primes that contain at least one decimal digit 8.
   The function a(n) returns the n‑th such prime (n ≥ 1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* check whether a seq_t integer contains digit 8 */
static bool has_digit8(seq_t x) {
    while (x != (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)8)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Cache for primes containing digit 8 */
static seq_t  *prime8_cache   = NULL;
static size_t  prime8_count   = 0;
static size_t  prime8_cap     = 0;
static idx_t   last_checked   = 1;   /* last number tested for primality */

/* growth helper from the utility library */
static void ensure_cache_capacity(size_t needed) {
    if (needed > prime8_cap) {
        if (prime8_cap == 0)
            prime8_cap = 10;
        while (prime8_cap < needed)
            prime8_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, prime8_cache, prime8_cap);
        prime8_cache = tmp;
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)0)    /* offset is 1; behaviour below offset not required */
        return (seq_t)0;

    /* generate primes until we have at least n in the cache */
    while (prime8_count < (size_t)n) {
        last_checked++;
        if (last_checked > (idx_t)2 && (last_checked % (idx_t)2) == (idx_t)0)
            continue;    /* skip even numbers after 2 */

        if (is_prime(last_checked)) {
            seq_t p = (seq_t)last_checked;
            if (has_digit8(p)) {
                ensure_cache_capacity(prime8_count + 1);
                prime8_cache[prime8_count++] = p;
            }
        }
    }
    return prime8_cache[n - 1];
}
