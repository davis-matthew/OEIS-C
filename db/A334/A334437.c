/*
 * A334437  Heinz number of the n-th reversed integer partition in graded
 *          lexicographical order.
 * https://oeis.org/A334437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 683, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Validated: standard, pass, 83 terms through n = 2626 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Heinz number of the n-th reversed integer partition in graded lexicographic order. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Recursive helper to generate all weakly increasing partitions of a given sum. */
static void gen_partitions(idx_t sum, idx_t min_part, seq_t current,
                           idx_t *cnt, idx_t n, seq_t *res, bool *found)
{
    if (*found) return;
    for (idx_t part = min_part; part <= sum; ++part) {
        seq_t prod = seq_mul(current, (seq_t)get_prime(part));
        if (part == sum) {
            ++(*cnt);
            if (*cnt == n) {
                *res = prod;
                *found = true;
                return;
            }
        } else {
            gen_partitions(sum - part, part, prod, cnt, n, res, found);
            if (*found) return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;         /* empty partition */
    idx_t counter  = 0;
    seq_t result   = 0;
    bool found     = false;

    /* Enumerate partitions by increasing total sum. */
    for (idx_t total = 1; !found; ++total) {
        gen_partitions(total, 1, (seq_t)1, &counter, n, &result, &found);
    }
    return result;
}
