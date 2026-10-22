/*
 * A354858  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that shares a factor with a(n-1) and both the sum a(n) +
 *          a(n-1) is distinct from all previous sums, a(i) + a(i-1),
 *          i=2..n-1, and the product a(n) * a(n-1) is distinct from all
 *          previous products, a(i) * a(i-1), i=2..n-1.
 * https://oeis.org/A354858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1, a(2) = 2; for n > 2, * a(n) = the smallest positive
 *            integer x such that * gcd(a(n-1), x) > 1, * the sum a(n-1)+x has
 *            not appeared as a sum of consecutive * terms before, and * the
 *            product a(n-1)*x has not appeared as a product of * consecutive
 *            terms before
 * Range:     u8 none, u16 none, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Parameters                                                        */
/* ------------------------------------------------------------------ */
#define MAX_N      20000          /* maximum n supported safely */
#define HASH_CAP   65536          /* must be a power of two, > 2*MAX_N */

/* ------------------------------------------------------------------ */
/* Static storage                                                     */
/* ------------------------------------------------------------------ */
static seq_t seq_arr[MAX_N + 1];   /* 1‑based indexing; seq_arr[1] = 1, seq_arr[2] = 2 */
static size_t seq_len = 0;          /* number of terms currently stored */

/* Hash tables for sums and products; 0 is the empty slot marker. */
static seq_t sum_hash[HASH_CAP];
static seq_t prod_hash[HASH_CAP];

/* ------------------------------------------------------------------ */
/* Utility: hash key (mod HASH_CAP)                                   */
/* ------------------------------------------------------------------ */
static inline size_t hash_key(seq_t key)
{
    /* Since HASH_CAP is a power of two, use modulo directly. */
    return (size_t)(key % (seq_t)HASH_CAP);
}

/* ------------------------------------------------------------------ */
/* Utility: check if key exists in hash table                         */
/* ------------------------------------------------------------------ */
static inline bool hset_contains(const seq_t tbl[HASH_CAP], seq_t key)
{
    if ((seq_t)0 == key) return false;          /* 0 never used as a real value */
    size_t idx = hash_key(key);
    while (tbl[idx]) {
        if (tbl[idx] == key) return true;
        idx = (idx + 1) & (HASH_CAP - 1);       /* linear probing */
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Utility: insert key into hash table                                */
/* ------------------------------------------------------------------ */
static inline void hset_insert(seq_t tbl[HASH_CAP], seq_t key)
{
    if ((seq_t)0 == key) return;
    size_t idx = hash_key(key);
    while (tbl[idx]) {
        if (tbl[idx] == key) return;            /* already present */
        idx = (idx + 1) & (HASH_CAP - 1);
    }
    tbl[idx] = key;
}

/* ------------------------------------------------------------------ */
/* Sequence initialization                                            */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    if (seq_len > 0) return;                 /* already initialized */

    seq_arr[1] = (seq_t)1;
    seq_arr[2] = (seq_t)2;
    seq_len  = 2;

    /* Record the first sum and product. */
    hset_insert(sum_hash, seq_arr[1] + seq_arr[2]);   /* 3 */
    hset_insert(prod_hash, seq_arr[1] * seq_arr[2]);  /* 2 */
}

/* ------------------------------------------------------------------ */
/* Main function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n = 0 */

    init_sequence();

    /* Return known value if already computed. */
    if ((size_t)n <= seq_len) return seq_arr[(size_t)n];

    /* Generate terms up to n. */
    for (size_t i = seq_len + 1; i <= (size_t)n; ++i) {
        seq_t prev = seq_arr[i - 1];
        seq_t cand = (seq_t)2;                /* smallest positive integer >=2 */

        /* Search for the smallest candidate satisfying all rules. */
        while (true) {
            if (gcd(prev, cand) == (seq_t)1) { /* need a common factor > 1 */
                cand++;
                continue;
            }
            seq_t sum  = prev + cand;
            seq_t prod = prev * cand;
            if (!hset_contains(sum_hash, sum) &&
                !hset_contains(prod_hash, prod)) {
                break;                       /* suitable candidate found */
            }
            cand++;                           /* try next integer */
        }

        seq_arr[i] = cand;
        seq_len = i;

        /* Record new sum and product. */
        hset_insert(sum_hash, prev + cand);
        hset_insert(prod_hash, prev * cand);
    }

    return seq_arr[(size_t)n];
}
