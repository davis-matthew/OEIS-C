/*
 * A370267  Numbers with an even number of prime factors not of the form 8m+-1
 *          (counting repetitions).
 * https://oeis.org/A370267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A370267.
   a(n) = the n-th positive integer that has an even number of prime
   factors (counted with multiplicity) that are NOT congruent to 1 or 7
   modulo 8 (i.e., primes ≡ 3 or 5 mod 8, plus the prime 2). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count prime factors of x that are NOT of the form 8k±1.
   2 is always counted.  Odd primes are counted only if p%8 is 3 or 5. */
static int count_non_pm1_factors(uint64_t x)
{
    int cnt = 0;
    /* factor 2 */
    while ((x & 1ULL) == 0ULL) {
        ++cnt;
        x >>= 1U;
    }
    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= x; p += 2ULL) {
        if (x % p == 0ULL) {
            if (p % 8ULL == 3ULL || p % 8ULL == 5ULL) {
                while (x % p == 0ULL) {
                    ++cnt;
                    x /= p;
                }
            } else {
                while (x % p == 0ULL) {
                    x /= p;
                }
            }
        }
    }
    if (x > 1ULL) {                 /* remaining prime factor */
        if (x % 8ULL == 3ULL || x % 8ULL == 5ULL)
            ++cnt;
    }
    return cnt;
}

/* Cache of computed sequence terms.  seq_cache[0] holds a(1). */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;          /* number of cached terms */
static size_t seq_cap   = 0;          /* allocated capacity */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (seq_len >= n)
        return;

    /* Grow the array if necessary. */
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Generate new terms starting from the integer after the last cached one. */
    seq_t next = (seq_len == 0) ? 1 : seq_cache[seq_len - 1] + 1;
    while (seq_len < n) {
        uint64_t uval = (uint64_t)next;        /* safe for n up to typical limits */
        int fl = count_non_pm1_factors(uval);
        if ((fl & 1) == 0) {                  /* even number → include in sequence */
            seq_cache[seq_len++] = next;
            if (seq_len == seq_cap) {          /* ensure capacity for next insertion */
                size_t new_cap = seq_cap * 2;
                seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
        }
        ++next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)               /* sequence is 1-indexed; n=0 is undefined */
        return 0;
    ensure_cache(n);
    return seq_cache[n - 1];
}
