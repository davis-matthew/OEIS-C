/*
 * A320771  Primes p for which p-1 and p+1 are Niven numbers.
 * https://oeis.org/A320771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 299, u32 n <= 11188, u64 n <= 11188,
 *            u128 n <= 11188, big any n
 * Validated: standard, pass, 68 terms through n = 9293 (2026-11-03)
 * Helpers:   digit_sum, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes p such that p-1 and p+1 are Niven numbers.
 * The sequence is generated lazily: each call to `a(n)` expands
 * the internal array until it contains at least n elements.
 * The algorithm tests consecutive integers, keeps those that
 * are prime and satisfy the Niven condition for both p−1 and p+1.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t *found = NULL;   /* array of found primes */
static size_t found_cnt = 0;  /* number of elements stored */
static size_t found_cap = 0;  /* current capacity of the array */
static seq_t next_num = 2;    /* next candidate integer to test */

static void grow(void) {
    if (found_cap == 0) {
        found_cap = 16;
        found = mem_new(seq_t, found_cap);
    } else {
        found_cap *= 2;
        found = mem_resize(seq_t, found, found_cap);
    }
}

/* test whether x is a Niven number: x % sumdigits(x) == 0 */
static bool is_niven(seq_t x) {
    seq_t s = digit_sum(x);
    return s != 0 && (x % s) == 0;
}

/* whether candidate is prime and both p-1 and p+1 are Niven */
static bool good_prime(seq_t p) {
    if (!is_prime((idx_t)p)) return false;
    return is_niven(p - 1) && is_niven(p + 1);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0; return 0 as placeholder */

    /* expand array until we have n terms */
    while (found_cnt < n) {
        /* find next prime with the property */
        while (!good_prime(next_num)) {
            next_num++;
        }
        /* store it */
        if (found_cnt == found_cap) grow();
        found[found_cnt++] = next_num;
        next_num++;   /* continue from next integer */
    }

    return found[n - 1];
}
