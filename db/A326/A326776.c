/*
 * A326776  a(n) is the smallest divisor of the n-th nonprime number
 *          (A018252(n)) not already in the sequence.
 * https://oeis.org/A326776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least divisor of the n‑th nonprime not already in the
 *            sequence
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements OEIS A326776.  It keeps a cache of all terms
   already produced (SEQ) and a bitmap of all integers that have appeared
   in the sequence (USED).  For each nonprime number N = A018252(k) we search
   its divisors in ascending order.  The first divisor that is not yet
   marked in USED is taken as a(k).  The algorithm works for any seq_t
   supplied by the build system and for any n ≥ 1.  It is O(n^2) in the
   worst case but fast enough for n ≈ 10000. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Global mutable state                                                */
/* ------------------------------------------------------------------ */
static seq_t *SEQ = NULL;           /* cache of sequence terms           */
static size_t SEQ_cap = 0;          /* allocated capacity of SEQ          */
static idx_t SEQ_len = 0;           /* number of terms already stored     */

static uint8_t *USED = NULL;        /* bitmap: USED[v] == 1 iff v was used */
static size_t USED_cap = 0;         /* number of elements allocated       */

static idx_t NEXT_CAND = 1;         /* next candidate for non‑prime       */

/* ------------------------------------------------------------------ */
/* Utility: ensure SEQ has at least 'needed' capacity                 */
/* ------------------------------------------------------------------ */
static void ensure_seq_cap(size_t needed)
{
    if (SEQ_cap < needed) {
        size_t new_cap = SEQ_cap ? SEQ_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(SEQ, new_cap * sizeof(seq_t));
        SEQ = tmp;
        SEQ_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Utility: ensure USED has at least 'needed' elements                */
/* ------------------------------------------------------------------ */
static void ensure_used_cap(idx_t needed)
{
    while ((size_t)needed > USED_cap) {
        size_t new_cap = USED_cap ? USED_cap * 2 : 128;
        if (new_cap < (size_t)needed) new_cap = (size_t)needed;
        uint8_t *tmp = (uint8_t *)realloc(USED, new_cap * sizeof(uint8_t));
        /* initialise the new part to 0 */
        if (tmp) {
            memset(tmp + USED_cap, 0, (new_cap - USED_cap) * sizeof(uint8_t));
            USED = tmp;
            USED_cap = new_cap;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Simple prime test (trial division), works for uint64_t values      */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Find the least divisor of a composite number N that is not yet used*/
/* ------------------------------------------------------------------ */
static seq_t least_unused_divisor_of(uint64_t n)
{
    seq_t result = 0;
    /* iterate d from 1 upwards while d*d <= n */
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        idx_t di = (idx_t)d;
        ensure_used_cap(di + 1);
        if (!USED[di]) {
            if (result == 0 || d < (uint64_t)result) result = (seq_t)d;
        }
        /* complementary divisor n/d */
        uint64_t d2 = n / d;
        if (d2 != d) {
            idx_t di2 = (idx_t)d2;
            ensure_used_cap(di2 + 1);
            if (!USED[di2]) {
                if (result == 0 || d2 < (uint64_t)result) result = (seq_t)d2;
            }
        }
    }
    /* At least n itself will be a divisor; it must be unused, so
       result never stays 0. */
    return result;
}

/* ------------------------------------------------------------------ */
/* Ensure the sequence is computed up to index n (n ≥ 1)               */
/* ------------------------------------------------------------------ */
static void build_sequence_upto(idx_t n)
{
    while (SEQ_len < n) {
        /* Find the next non‑prime candidate */
        uint64_t cand = NEXT_CAND;
        while (is_prime_uint64(cand)) ++cand;
        NEXT_CAND = cand + 1;

        /* Determine a(SEQ_len+1) */
        seq_t val = least_unused_divisor_of(cand);

        /* Store it and mark as used */
        ensure_seq_cap(SEQ_len + 1);
        SEQ[SEQ_len++] = val;

        idx_t vi = (idx_t)val;
        ensure_used_cap(vi + 1);
        USED[vi] = 1;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset is 1 */
    build_sequence_upto(n);
    return SEQ[n - 1];
}
