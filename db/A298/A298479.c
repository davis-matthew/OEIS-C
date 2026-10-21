/*
 * A298479  Matula-Goebel numbers of rooted trees in which all positive
 *          outdegrees are different.
 * https://oeis.org/A298479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A298479:
   Matula–Goebel numbers of rooted trees in which all positive outdegrees are different.
   The function returns a(n) for index n ≥ 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper structures and globals ---------- */

struct MemoEntry {
    bool computed;   /* value already computed for x */
    bool valid;      /* is the tree with Matula number x valid? */
    uint64_t mask;   /* bit i set ⇔ the subtree contains a node of positive outdegree i+1 */
};

static uint64_t *spf          = NULL;  /* smallest prime factor of each integer */
static uint32_t *prime_index  = NULL;  /* index of a prime (p_n -> n) */
static struct MemoEntry *memo = NULL;  /* memoisation table for analyze() */

static idx_t current_limit   = 0;  /* maximum integer for which tables are valid */

/* ---------- extend precomputed tables to at least n ---------- */
static void extend_limit(idx_t n)
{
    if (n <= current_limit)
        return;

    /* grow to at least twice the current size or n, whichever is larger */
    idx_t new_limit = current_limit == 0 ? n : current_limit;
    while (new_limit < n) new_limit <<= 1;

    /* reallocate arrays */
    spf          = (uint64_t*)realloc(spf,          (new_limit + 1) * sizeof(uint64_t));
    prime_index  = (uint32_t*)realloc(prime_index,  (new_limit + 1) * sizeof(uint32_t));
    memo         = (struct MemoEntry*)realloc(memo, (new_limit + 1) * sizeof(struct MemoEntry));

    /* initialisation of new entries */
    for (idx_t i = current_limit + 1; i <= new_limit; ++i) {
        spf[i]          = 0;
        prime_index[i]  = 0;
        memo[i].computed = false;
    }
    if (current_limit == 0) {
        spf[0] = 0;
        spf[1] = 1;
    }

    /* sieve to fill spf for numbers > current_limit */
    for (idx_t i = current_limit + 1; i <= new_limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i <= new_limit / i) {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* compute prime_index for all primes up to new_limit */
    idx_t idx = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            ++idx;
        }
        prime_index[i] = (uint32_t)idx;     /* 0 for non‑primes */
    }

    current_limit = new_limit;
}

/* ---------- recursive analysis of a tree with Matula number x ---------- */
static bool analyze(idx_t x, uint64_t *out_mask)
{
    if (memo[x].computed) {
        *out_mask = memo[x].mask;
        return memo[x].valid;
    }

    bool result = true;
    uint64_t mask = 0;

    if (x == 1) {                /* leaf */
        mask = 0;
    } else {
        idx_t tmp = x;
        idx_t total_d = 0;
        uint64_t union_mask = 0;
        bool local_valid = true;

        /* factorise x using smallest prime factors */
        while (tmp > 1) {
            idx_t p = spf[tmp];
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            idx_t child = prime_index[p];      /* Matula number of the child subtree */
            uint64_t child_mask;
            bool child_valid = analyze(child, &child_mask);
            if (!child_valid) { local_valid = false; break; }

            /* repeated non‑leaf children would duplicate a positive outdegree */
            if (child != 1 && exp > 1) { local_valid = false; break; }

            total_d += exp;

            if (child_mask != 0) {
                if ((union_mask & child_mask) != 0) { local_valid = false; break; }
                union_mask |= child_mask;
            }
        }

        if (local_valid) {
            if (total_d == 0 || total_d > 63) {
                local_valid = false;
            } else {
                uint64_t bit = 1ULL << (total_d - 1);
                if ((union_mask & bit) != 0) {
                    local_valid = false;
                } else {
                    mask = union_mask | bit;
                }
            }
        }

        result = local_valid;
    }

    memo[x].computed = true;
    memo[x].valid    = result;
    memo[x].mask     = mask;
    *out_mask = mask;
    return result;
}

/* ---------- sequence generation ---------- */
static seq_t *seq_terms = NULL;   /* stored Matula numbers that satisfy the property */
static size_t seq_cap   = 0;      /* current capacity of seq_terms */
static size_t seq_cnt   = 0;      /* how many terms have been found */
static idx_t  last_checked = 0;   /* largest integer that has been tested */

seq_t a(idx_t n)
{
    /* one‑time initialisation */
    static bool initialized = false;
    if (!initialized) {
        extend_limit(1024);                  /* start with a modest limit */
        seq_cap    = 128;
        seq_terms  = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (seq_cnt < (size_t)n) {
        if (last_checked >= current_limit) {
            extend_limit(current_limit * 2 + 1);
        }
        ++last_checked;                     /* candidate integer */

        uint64_t dummy;
        if (analyze(last_checked, &dummy)) { /* valid tree found */
            if (seq_cnt + 1 > seq_cap) {
                seq_cap <<= 1;
                seq_terms = (seq_t*)realloc(seq_terms, seq_cap * sizeof(seq_t));
            }
            seq_terms[seq_cnt++] = (seq_t)last_checked;
        }
    }

    return seq_terms[n - 1];
}
