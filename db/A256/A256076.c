/*
 * A256076  Non-palindromic balanced primes.
 * https://oeis.org/A256076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8343 (2026-11-01)
 * Helpers:   get_digits, is_palindrome_base, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑palindromic balanced primes (A256076)
   Computed by searching for primes, testing for a
   balanced digit sum and checking that they are not
   palindromic.  All state is cached so any order
   of calls works. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *balanced_primes = NULL;   /* cached values */
static size_t bal_count = 0;            /* number of primes cached */
static size_t bal_capacity = 0;         /* current storage capacity */
static idx_t next_candidate = 3;        /* next odd integer to test (2 is never useful) */

/* Return true if seq_t n has zero weighted digit sum. */
static bool is_balanced(seq_t n) {
    uint8_t buf[256];
    size_t len;
    get_digits(n, buf, &len);
    if (len == 0) return false;

    long long weighted = 0;
    for (size_t i = 0; i < len; ++i) {
        int w = (int)(len - 1 - 2 * i);
        weighted += (long long)buf[i] * w;
    }
    return weighted == 0;
}

/* Expand storage to accommodate at least need elements. */
static void ensure_capacity(size_t need) {
    if (bal_capacity >= need) return;
    size_t new_cap = bal_capacity == 0 ? 16 : bal_capacity << 1;
    while (new_cap < need) new_cap <<= 1;
    balanced_primes = seq_realloc(balanced_primes, bal_capacity, new_cap);
    bal_capacity = new_cap;
}

/* Fill the cache with at least n non‑palindromic balanced primes. */
static void ensure_balanced(idx_t n) {
    if (bal_count >= (size_t)n) return;

    ensure_capacity(n);
    while (bal_count < (size_t)n) {
        seq_t cand_seq = (seq_t)next_candidate;
        if (is_prime(next_candidate) &&
            !is_palindrome_base(cand_seq, 10) &&
            is_balanced(cand_seq)) {
            balanced_primes[bal_count++] = cand_seq;
        }
        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* out of range; return 0 */
    ensure_balanced(n);
    return balanced_primes[n - 1];
}
