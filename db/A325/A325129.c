/*
 * A325129  Heinz numbers of integer partitions into nonsquares (A087153).
 * https://oeis.org/A325129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest product of primes p_k where k is not a
 *            perfect square (Heinz numbers of integer partitions into
 *            nonsquares)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* ---------- Precompute allowed primes (indices not perfect squares) ---------- */

static uint32_t *allowed_primes = NULL;   /* primes with square‑free indices */
static size_t primes_cnt = 0;

static void init_primes(void) {
    if (allowed_primes) return;           /* already initialized */

    /* Generate all primes up to a reasonable limit. 200 000 is more than enough
       to provide all primes needed for the first 10 000 Heinz numbers. */
    const idx_t LIMIT = 200000;
    uint64_t *all = NULL;
    size_t all_cnt;
    build_primes_upto(LIMIT, &all, &all_cnt);

    allowed_primes = (uint32_t *)malloc(all_cnt * sizeof(uint32_t));
    primes_cnt = 0;

    for (size_t i = 0; i < all_cnt; ++i) {
        idx_t idx = (idx_t)i + 1;       /* prime index (1‑based) */

        /* fast integer square root: for small values a loop is fine */
        idx_t r = 0;
        while ((r + 1) * (r + 1) <= idx) ++r;

        if (r * r == idx) continue;    /* skip perfect‑square indices */
        allowed_primes[primes_cnt++] = (uint32_t)all[i];
    }
    free(all);
}

/* ---------- Sequence storage and generation ---------- */

static seq_t *seq = NULL;     /* sequence values, 1‑indexed in terms of OEIS */
static size_t seq_len = 0;    /* number of values stored in seq (≥1) */
static size_t seq_cap = 0;    /* allocated capacity of seq */

static size_t *prime_idx = NULL;  /* for each prime: index into seq for next candidate */

static void ensure_sequence(idx_t n) {
    if (seq_len >= (size_t)n) return;  /* already have enough terms */

    init_primes();
    if (!prime_idx) {
        prime_idx = (size_t *)malloc(primes_cnt * sizeof(size_t));
        for (size_t i = 0; i < primes_cnt; ++i) prime_idx[i] = 0;
    }

    /* Allocate enough space for seq */
    if (!seq) {
        seq_cap = (size_t)n * 2;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_cap < (size_t)n) {
        seq_cap = (size_t)n * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }

    if (seq_len == 0) {          /* initialise with the empty partition */
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* Generate new terms until we have at least n */
    while (seq_len < (size_t)n) {
        seq_t next = 0;
        bool first = true;

        /* Find the minimum candidate among all primes */
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t cand = seq[prime_idx[i]] * (seq_t)allowed_primes[i];
            if (first || cand < next) {
                next = cand;
                first = false;
            }
        }

        /* Append the new term */
        seq[seq_len++] = next;

        /* Advance indices for primes that generated this term (to avoid duplicates) */
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t cand = seq[prime_idx[i]] * (seq_t)allowed_primes[i];
            if (cand == next) ++prime_idx[i];
        }
    }
}

/* ---------- Public API ---------- */

seq_t a(idx_t n) {
    ensure_sequence(n);
    return seq[(size_t)n - 1];   /* OEIS offset 1 → seq index 0 */
}
