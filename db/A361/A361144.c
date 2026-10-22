/*
 * A361144  Lexicographically earliest sequence of positive integers such that
 *          the sums Sum_{i = 1+k*2^e..(k+1)*2^e} a(i) with k, e >= 0 are all
 *          distinct.
 * https://oeis.org/A361144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is generated greedily: for each index i (1‑based)
 *            choose the smallest positive integer v that is not already used
 *            and for which all block sums that become defined at position i
 *            remain distinct. A block is a contiguous interval whose length
 *            is a power of two; for i the blocks that bec...
 * Range:     u8 n <= 31, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   hs_hash_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/hs_hash_value.h"

/* Forward declaration of seq_t and idx_t types supplied by the build. */
extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void free(void *);

seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/*   Utility: simple open addressing hash set for seq_t values   */
/* ------------------------------------------------------------ */
typedef struct {
    seq_t *table;   /* table array, 0 == empty slot             */
    size_t cap;     /* table capacity (power of two)            */
    size_t mask;    /* cap - 1, for masking index              */
    size_t sz;      /* number of elements stored                */
} HashSet;

static void hs_init(HashSet *hs, size_t init_cap) {
    /* init_cap should be power of two and > 0 */
    hs->cap = init_cap;
    hs->mask = init_cap - 1;
    hs->sz = 0;
    hs->table = (seq_t *)calloc(hs->cap, sizeof(seq_t));
}

static void hs_free(HashSet *hs) {
    free(hs->table);
    hs->table = NULL;
    hs->cap = hs->mask = hs->sz = 0;
}

static void hs_rehash(HashSet *hs) {
    size_t new_cap = hs->cap << 1;
    seq_t *new_table = (seq_t *)calloc(new_cap, sizeof(seq_t));
    size_t new_mask = new_cap - 1;
    for (size_t i = 0; i < hs->cap; ++i) {
        seq_t val = hs->table[i];
        if (val != 0) {
            size_t h = hs_hash_value(val) & new_mask;
            while (new_table[h] != 0) h = (h + 1) & new_mask;
            new_table[h] = val;
        }
    }
    free(hs->table);
    hs->table = new_table;
    hs->cap = new_cap;
    hs->mask = new_mask;
}

static bool hs_contains(const HashSet *hs, seq_t v) {
    if (v == 0) return false;          /* 0 is never a value in the sequence */
    size_t h = hs_hash_value(v) & hs->mask;
    while (true) {
        seq_t cur = hs->table[h];
        if (cur == 0) return false;
        if (cur == v) return true;
        h = (h + 1) & hs->mask;
    }
}

static bool hs_insert(HashSet *hs, seq_t v) {
    if (v == 0) return false;
    if (hs->sz * 3 >= hs->cap * 2) hs_rehash(hs);
    size_t h = hs_hash_value(v) & hs->mask;
    while (true) {
        seq_t cur = hs->table[h];
        if (cur == 0) {
            hs->table[h] = v;
            hs->sz++;
            return true;
        }
        if (cur == v) return false;   /* already present */
        h = (h + 1) & hs->mask;
    }
}

/* ------------------------------------------------------------ */
/*                Sequence generation state                      */
/* ------------------------------------------------------------ */
static seq_t *seq_buf    = NULL;   /* a[0 .. len-1]          */
static size_t seq_cap    = 0;
static idx_t  seq_len    = 0;      /* number of values computed */

static seq_t *pref_buf   = NULL;   /* prefix sums             */
static size_t pref_cap   = 0;
static idx_t  pref_len   = 0;      /* pref_len == seq_len + 1 */

static HashSet used_set;

/* Ensure buffers can hold up to 'need' elements. */
static void ensure_buffers(idx_t need) {
    if (need <= seq_len) return;
    /* Grow sequence buffer */
    size_t required_seq_cap = 2 * (size_t)need;     /* safety factor */
    if (required_seq_cap > seq_cap) {
        seq_cap = required_seq_cap;
        seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
    }
    /* Grow prefix buffer */
    size_t required_pref_cap = required_seq_cap + 1;
    if (required_pref_cap > pref_cap) {
        pref_cap = required_pref_cap;
        pref_buf = (seq_t *)realloc(pref_buf, pref_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------ */
/*                 Main function a(idx_t n)                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* by convention, unused */

    ensure_buffers(n);
    /* Grow used set if it is the first time */
    static bool initialized = false;
    if (!initialized) {
        hs_init(&used_set, 16384);   /* start with 16K cells (power of two) */
        /* initialise prefix sum array */
        pref_buf[0] = 0;
        pref_len = 1;
        initialized = true;
    }
    /* compute values until seq_len >= n */
    for (idx_t i = seq_len+1; i <= n; ++i) {
        /* candidate search for a[i] */
        seq_t candidate = 1;
        while (true) {
            bool conflict = false;
            if (hs_contains(&used_set, candidate)) {
                conflict = true;
            } else {
                /* test all blocks that finish at position i */
                idx_t L = 1;
                while (L <= i) {
                    if ((i & (L - 1)) == 0) {   /* i % L == 0 */
                        /* sum of block without candidate = pref_buf[i-1] - pref_buf[i-L] */
                        seq_t without = pref_buf[i-1] - pref_buf[i-L];
                        seq_t block_sum = without + candidate;
                        if (hs_contains(&used_set, block_sum)) {
                            conflict = true;
                            break;
                        }
                    }
                    if (L > i/2) break;  /* avoid overflow in shift */
                    L <<= 1;
                }
            }
            if (!conflict) break;
            ++candidate;
        }
        /* Record the chosen value */
        seq_buf[seq_len] = candidate;
        seq_len++;

        /* Update prefix sums */
        pref_buf[i] = pref_buf[i-1] + candidate;
        /* Add the leaf value to used set */
        hs_insert(&used_set, candidate);

        /* Add sums of blocks that finish at i */
        idx_t L = 1;
        while (L <= i) {
            if ((i & (L - 1)) == 0) {
                seq_t block_sum = pref_buf[i] - pref_buf[i-L];
                hs_insert(&used_set, block_sum);
            }
            if (L > i/2) break;
            L <<= 1;
        }
    }
    return seq_buf[n-1];
}
