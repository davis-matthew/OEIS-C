/*
 * A247751  Numbers in decimal representation, such that in Danish their
 *          digits are in alphabetic order.
 * https://oeis.org/A247751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 2468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A247751: Numbers in decimal representation, such that in Danish
   their digits are in alphabetic order.  The function generates
   the sequence on demand, storing the generated terms in a static
   cache so subsequent calls are fast. */

#include <stdlib.h>

static const int rank_arr[10] = {
    /* 0 */ 4,
    /* 1 */ 0,
    /* 2 */ 8,
    /* 3 */ 9,
    /* 4 */ 2,
    /* 5 */ 1,
    /* 6 */ 6,
    /* 7 */ 7,
    /* 8 */ 5,
    /* 9 */ 3
};

static seq_t      *cache      = NULL;   /* array of generated terms      */
static size_t      cache_cnt  = 0;      /* number of terms stored         */
static size_t      cache_cap  = 0;      /* capacity of the array          */
static int         max_len_gen = 0;     /* maximum length already generated */

/* Append a new value to the cache, expanding the array if necessary. */
static void push_to_cache(seq_t val) {
    if (cache_cnt == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);                    /* abort on OOM */
        cache      = new_ptr;
        cache_cap  = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* Recursive generator for all numbers of a given length that satisfy
   the monotone rank condition.  It produces values in numeric order. */
static void gen_len(int len, int pos, seq_t val, int last_rank) {
    if (pos == len) {
        push_to_cache(val);
        return;
    }
    int start = (pos == 0) ? 1 : 0;                 /* avoid leading 0 */
    for (int d = start; d <= 9; ++d) {
        if (rank_arr[d] >= last_rank) {
            gen_len(len, pos + 1, val * (seq_t)10 + (seq_t)d, rank_arr[d]);
        }
    }
}

/* Generate all numbers of length 'len' and update the maximum generated
   length.  After this call, the cache contains all valid numbers with
   lengths up to 'len' (except 0 which was inserted separately). */
static void generate_len(int len) {
    if (len <= max_len_gen) return;
    for (int l = max_len_gen + 1; l <= len; ++l) {
        gen_len(l, 0, (seq_t)0, -1);   /* -1 allows any first digit rank */
    }
    max_len_gen = len;
}

/* Ensure that the cache contains at least 'n' terms. */
static void ensure_terms(idx_t n) {
    if (cache == NULL) {                    /* first call: initialise */
        cache = (seq_t *)malloc(8 * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap  = 8;
        cache_cnt  = 0;
        max_len_gen = 0;
        push_to_cache((seq_t)0);            /* a(1) = 0 */
    }
    while (cache_cnt < n) {
        generate_len(max_len_gen + 1);       /* extend by one digit length */
    }
}

/* External function required by the interface. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined but safe */
    ensure_terms(n);
    return cache[n - 1];                    /* offset 1 */
}
