/*
 * A320587  Primes that are not Base-3 deletable primes (written in base 10).
 * https://oeis.org/A320587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 906, u32 n <= 906, u64 n <= 906,
 *            u128 n <= 906, big any n
 * Helpers:   append_candidate, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the OEIS sequence A320587: 
   primes that are NOT base-3 deletable primes.
   It enumerates primes in ascending order, tests each for
   base‑3 deletability, and returns the *n*‑th non‑deletable prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/append_candidate.h"

/* cache of deletable primes, sorted ascending */
static seq_t *deletable = NULL;
static size_t deletable_cnt = 0, deletable_cap = 0;

/* cache of non‑deletable primes (used to answer a(n)) */
static seq_t *non_deletable = NULL;
static size_t non_deletable_cnt = 0, non_deletable_cap = 0;

/* next integer to test for primality */
static idx_t next_candidate = 2;

/* binary search in the deletable cache */
static bool is_deletable_cached(seq_t val)
{
    size_t lo = 0, hi = deletable_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (deletable[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return (lo < deletable_cnt && deletable[lo] == val);
}

/* test whether a prime p (idx_t) is a base‑3 deletable prime */
static bool is_deletable_prime(idx_t p)
{
    /* convert p to base‑3 digits (most significant first) */
    uint8_t rev[64];
    size_t len = 0;
    idx_t t = p;
    do {
        rev[len++] = (uint8_t)(t % 3);
        t /= 3;
    } while (t);

    uint8_t dig[64];
    for (size_t i = 0; i < len; ++i) dig[i] = rev[len - 1 - i];

    /* single‑digit base‑3 numbers are trivially deletable (empty string) */
    if (len == 1) return true;

    /* try each legal deletion; existence of one successful deletion
       makes the number deletable */
    for (size_t i = 0; i < len; ++i) {
        if (i == 0 && len > 1 && dig[1] == 0) continue; /* leading zero not allowed */

        seq_t q = 0;
        for (size_t j = 0; j < len; ++j) {
            if (j == i) continue;
            q = q * 3 + dig[j];
        }

        /* q > 0 here (len>1) */
        if (!is_prime((idx_t)q)) continue;
        if (is_deletable_cached(q)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but safe return */

    /* generate primes until we have at least n non‑deletable primes */
    while (non_deletable_cnt < n) {
        /* skip composites */
        while (!is_prime(next_candidate)) ++next_candidate;

        /* test deletability */
        if (is_deletable_prime(next_candidate)) {
            /* store in deletable cache */
            if (deletable_cnt == deletable_cap) {
                size_t new_cap = deletable_cap ? deletable_cap * 2 : 4;
                deletable = (seq_t *)realloc(deletable, new_cap * sizeof(seq_t));
                deletable_cap = new_cap;
            }
            deletable[deletable_cnt++] = (seq_t)next_candidate;
        } else {
            /* store in non‑deletable list */
            append_candidate(&non_deletable, &non_deletable_cnt,
                             &non_deletable_cap, (seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return non_deletable[n - 1];
}
