/*
 * A340102  Number of factorizations of 2n + 1 into an odd number of odd
 *          factors > 1.
 * https://oeis.org/A340102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of factorizations of 2n+1 into an odd number of
 *            odd factors > 1
 * Range:     u8 n <= 6129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Simple open‑addressing hash table for memoisation            */
/* ------------------------------------------------------------ */

#define HASH_SIZE (1U << 20)                /* 1,048,576 entries */

typedef struct {
    uint64_t key;
    seq_t    val;
    unsigned used;
} hash_entry_t;

static hash_entry_t odd_table[HASH_SIZE];
static hash_entry_t even_table[HASH_SIZE];

static inline uint64_t hash64(uint64_t x)
{
    return (x * 11400714819323198485ULL) & (HASH_SIZE - 1);
}

static inline int find_entry(hash_entry_t *table, uint64_t key, seq_t *out)
{
    uint64_t idx = hash64(key);
    for (;;) {
        if (!table[idx].used) return 0;
        if (table[idx].key == key) {
            *out = table[idx].val;
            return 1;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
}

static inline void insert_entry(hash_entry_t *table, uint64_t key, seq_t val)
{
    uint64_t idx = hash64(key);
    for (;;) {
        if (!table[idx].used) {
            table[idx].used = 1;
            table[idx].key   = key;
            table[idx].val   = val;
            return;
        }
        if (table[idx].key == key) {
            table[idx].val = val;
            return;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
}

/* ------------------------------------------------------------ */
/*  Divisor lists for odd numbers → memoisation of factors      */
/* ------------------------------------------------------------ */

static idx_t **divs = NULL;     /* pointer to array of divisor lists */
static size_t *divcnt = NULL;   /* number of divisors per odd number */
static idx_t global_max = 0;    /* maximum n for which divs[] is valid */

static void precompute_divisors(idx_t up_to)
{
    if (up_to <= global_max) return;

    /* resize arrays */
    divs   = (idx_t**)realloc(divs,   (up_to + 1) * sizeof(idx_t*));
    divcnt = (size_t*) realloc(divcnt, (up_to + 1) * sizeof(size_t));
    if (!divs || !divcnt) { exit(EXIT_FAILURE); }

    /* initialise new entries */
    for (idx_t i = global_max + 1; i <= up_to; ++i) {
        divs[i]   = NULL;
        divcnt[i] = 0;
    }

    for (idx_t i = global_max + 1; i <= up_to; ++i) {
        if ((i & 1) == 0 || i < 3) continue;      /* only odd ≥3 */

        size_t cap = 8;
        idx_t *tmp = (idx_t*)malloc(cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        size_t ct = 0;

        for (idx_t d = 3; d * d <= i; d += 2) {
            if (i % d == 0) {
                if (ct == cap) {
                    cap *= 2;
                    tmp = (idx_t*)realloc(tmp, cap * sizeof(idx_t));
                    if (!tmp) exit(EXIT_FAILURE);
                }
                tmp[ct++] = d;
                idx_t e = i / d;
                if (e != d) {
                    if (ct == cap) {
                        cap *= 2;
                        tmp = (idx_t*)realloc(tmp, cap * sizeof(idx_t));
                        if (!tmp) exit(EXIT_FAILURE);
                    }
                    tmp[ct++] = e;
                }
            }
        }
        if (ct == cap) {
            cap++;
            tmp = (idx_t*)realloc(tmp, cap * sizeof(idx_t));
            if (!tmp) exit(EXIT_FAILURE);
        }
        tmp[ct++] = i;          /* include the number itself */
        divs[i]   = tmp;
        divcnt[i] = ct;
    }

    global_max = up_to;
}

/* ------------------------------------------------------------ */
/*  Recursive counting with memoisation                        */
/* ------------------------------------------------------------ */

static seq_t odd_count(idx_t rem, idx_t minf);
static seq_t even_count(idx_t rem, idx_t minf);

static seq_t odd_count(idx_t rem, idx_t minf)
{
    if (rem == 1) return 0;

    uint64_t key = ((uint64_t)rem << 32) | (uint64_t)minf;
    seq_t cached;
    if (find_entry(odd_table, key, &cached)) return cached;

    seq_t res = 0;
    precompute_divisors(rem);
    for (size_t k = 0; k < divcnt[rem]; ++k) {
        idx_t d = divs[rem][k];
        if (d < minf) continue;
        idx_t q = rem / d;
        res += even_count(q, d);
    }

    insert_entry(odd_table, key, res);
    return res;
}

static seq_t even_count(idx_t rem, idx_t minf)
{
    if (rem == 1) return 1;

    uint64_t key = ((uint64_t)rem << 32) | (uint64_t)minf;
    seq_t cached;
    if (find_entry(even_table, key, &cached)) return cached;

    seq_t res = 0;
    precompute_divisors(rem);
    for (size_t k = 0; k < divcnt[rem]; ++k) {
        idx_t d = divs[rem][k];
        if (d < minf) continue;
        idx_t q = rem / d;
        res += odd_count(q, d);
    }

    insert_entry(even_table, key, res);
    return res;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    seq_t M = ((seq_t)n << 1) + 1;     /* 2n + 1 */
    precompute_divisors((idx_t)M);
    return odd_count((idx_t)M, 3);     /* smallest odd factor > 1 is 3 */
}
