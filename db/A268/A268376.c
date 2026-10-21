/*
 * A268376  Numbers n for which A001222(n) > A267116(n).
 * https://oeis.org/A268376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268376
 * Definition:
 *    a(n) = the n-th integer such that in its prime factorisation
 *           n = ∏ p_i^e_i there is at least one pair (i≠j) with
 *           (e_i & e_j) ≠ 0   (bitwise AND of binary exponents non-zero).
 * The code generates terms by scanning numbers sequentially and testing
 * the condition.  Local cache allows arbitrary order of calls.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache = NULL;   /* dynamic array of terms */
static size_t seq_len  = 0;       /* number of terms stored   */
static size_t seq_cap  = 0;       /* current capacity        */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Append a value to the sequence cache, expanding capacity if needed. */
static void append_candidate(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1); /* abort on allocation failure */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Check whether a uint64_t value qualifies for the sequence. */
static bool qualifies(uint64_t m)
{
    uint64_t exps[64];
    size_t ex_cnt = 0;

    /* factor 2 separately */
    if (m & 1) {
        /* odd number, skip */
    } else {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            ++e;
            m >>= 1;
        }
        if (e) {
            exps[ex_cnt++] = e;
        }
    }

    /* trial division by odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            exps[ex_cnt++] = e;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        exps[ex_cnt++] = 1;
    }

    if (ex_cnt < 2) return false;

    for (size_t i = 0; i < ex_cnt; ++i) {
        for (size_t j = i + 1; j < ex_cnt; ++j) {
            if ((exps[i] & exps[j]) != 0) {
                return true;
            }
        }
    }
    return false;
}

/* Return the n-th term of the sequence (1-based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for index 0, but return 0 */
    }

    /* Ensure cache contains at least n terms */
    while (seq_len < (size_t)n) {
        if (qualifies(next_candidate)) {
            append_candidate((seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
