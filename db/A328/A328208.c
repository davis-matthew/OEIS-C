/*
 * A328208  Zeckendorf-Niven numbers: numbers divisible by the number of terms
 *          in their Zeckendorf representation (A007895).
 * https://oeis.org/A328208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 8151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Zeckendorf-Niven numbers:
   a(n) = n-th integer divisible by the number of terms in its Zeckendorf representation
*/

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc */
#include <stdbool.h>  /* for bool */

/* static cache for already computed terms */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t cache_last = 0;

/* static Fibonacci list for Zeckendorf decomposition */
static seq_t *fibs = NULL;
static size_t fib_cap = 0;
static size_t fib_len = 0;

/* enlarge fibonacci storage if needed */
static void ensure_fib_cap(size_t needed) {
    if (needed > fib_cap) {
        size_t new_cap = fib_cap ? fib_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_fibs = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
        fibs = new_fibs;
        fib_cap = new_cap;
    }
}

/* lazy initializer for Fibonacci list */
static void init_fibs(void) {
    if (fib_len == 0) {
        ensure_fib_cap(2);
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)2;
        fib_len = 2;
    }
}

/* number of terms in Zeckendorf representation of n */
static unsigned int zeckendorf_terms(seq_t n) {
    init_fibs();

    /* grow Fibonacci list until the last element > n */
    while (fibs[fib_len - 1] <= n) {
        seq_t next = fibs[fib_len - 1] + fibs[fib_len - 2];
        ensure_fib_cap(fib_len + 1);
        fibs[fib_len++] = next;
    }

    /* greedy decomposition */
    unsigned int count = 0;
    seq_t remaining = n;

    for (size_t i = fib_len; i > 0; --i) {
        size_t idx = i - 1;
        if (fibs[idx] <= remaining) {
            remaining -= fibs[idx];
            ++count;
            if (remaining == 0) break;
        }
    }
    return count;
}

/* find next Zeckendorf-Niven number after cache_last */
static seq_t find_next_zn(void) {
    seq_t candidate = cache_last + (seq_t)1;
    while (true) {
        unsigned int terms = zeckendorf_terms(candidate);
        if (terms != 0 && (candidate % (seq_t)terms == 0)) {
            return candidate;
        }
        ++candidate;
    }
}

/* enlarge cache if needed */
static void ensure_cache_capacity(size_t needed) {
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = new_cache;
        cache_cap = new_cap;
    }
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but return 0 */

    /* extend cache up to n */
    while (n > cache_len) {
        seq_t next = find_next_zn();
        ensure_cache_capacity(cache_len + 1);
        seq_cache[cache_len++] = next;
        cache_last = next;
    }
    return seq_cache[n - 1];
}
