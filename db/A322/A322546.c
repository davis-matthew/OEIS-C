/*
 * A322546  Numbers k such that every integer partition of k contains a 1 or a
 *          prime power.
 * https://oeis.org/A322546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file implements a(idx_t n) → seq_t for OEIS A322546.
 * The sequence consists of all positive integers k for which every
 * integer partition of k contains at least one part that is a 1 or a
 * prime power.  Equivalently, k is NOT representable as a sum of
 * composite numbers that are NOT prime powers (the “bad parts”).
 * The algorithm incrementally scans the integers, maintains a list
 * of bad parts and a reachability table for sums of bad parts,
 * and records the numbers that are not reachable.  The function
 * works for any n ≥ 1, is re‑entrant (the state is memoised)
 * and uses only integer arithmetic.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"

static bool is_bad_part(idx_t x) {
    if (x < 6) return false;                /* 1–5 are not composite */
    if (is_prime(x)) return false;          /* primes are allowed */
    uint64_t spf = _smallest_prime_factor((uint64_t)x);
    idx_t t = x;
    while (t % spf == 0) t /= spf;
    return (t != 1);                        /* composite and not prime‑power */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* outside OEIS offset – undefined */

    /* ---------- static memoised state ---------- */
    static seq_t *good = NULL;        /* list of good numbers found so far */
    static size_t good_len = 0, good_cap = 0;

    static idx_t *bad = NULL;         /* list of bad parts */
    static size_t bad_len = 0, bad_cap = 0;

    static bool *reach = NULL;        /* reach[i] = true iff i is representable */
    static size_t reach_cap = 0;

    static idx_t cur = 0;              /* last integer processed */

    /* initialise on first call */
    if (!good) {
        good_cap = 16;
        good = (seq_t*)malloc(good_cap * sizeof(seq_t));
        if (!good) exit(1);
        good_len = 0;

        bad_cap = 16;
        bad = (idx_t*)malloc(bad_cap * sizeof(idx_t));
        if (!bad) exit(1);
        bad_len = 0;

        reach_cap = 1;
        reach = (bool*)malloc(reach_cap * sizeof(bool));
        if (!reach) exit(1);
        reach[0] = true; /* empty sum */
        cur = 0;
    }

    /* extend the search until we have n good numbers */
    while (good_len < (size_t)n) {
        ++cur;                     /* new integer to examine */

        /* grow reach array if needed */
        if (reach_cap <= (size_t)cur) {
            size_t new_cap = reach_cap * 2;
            if (new_cap <= (size_t)cur) new_cap = (size_t)cur + 1;
            bool *tmp = (bool*)realloc(reach, new_cap * sizeof(bool));
            if (!tmp) exit(1);
            for (size_t i = reach_cap; i < new_cap; ++i) tmp[i] = false;
            reach = tmp;
            reach_cap = new_cap;
        }

        /* test whether cur is a bad part */
        bool badp = false;
        if (cur >= 6) {
            if (!is_prime(cur)) {
                uint64_t spf = _smallest_prime_factor((uint64_t)cur);
                idx_t t = cur;
                while (t % spf == 0) t /= spf;
                if (t != 1) badp = true;
            }
        }

        /* add bad part to list if applicable */
        if (badp) {
            if (bad_len == bad_cap) {
                size_t new_cap = bad_cap ? bad_cap * 2 : 16;
                idx_t *tmp = (idx_t*)realloc(bad, new_cap * sizeof(idx_t));
                if (!tmp) exit(1);
                bad = tmp;
                bad_cap = new_cap;
            }
            bad[bad_len++] = cur;
        }

        /* determine reachability of cur */
        bool reachable = false;
        for (size_t i = 0; i < bad_len; ++i) {
            idx_t p = bad[i];
            if (p > cur) break;
            if (reach[(size_t)(cur - p)]) {
                reachable = true;
                break;
            }
        }
        reach[(size_t)cur] = reachable;

        /* cur is good iff it cannot be represented by bad parts */
        if (!reachable) {
            if (good_len == good_cap) {
                size_t new_cap = good_cap ? good_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(good, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                good = tmp;
                good_cap = new_cap;
            }
            good[good_len++] = (seq_t)cur;
        }
    }

    return good[n - 1];
}
