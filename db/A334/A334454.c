/*
 * A334454  Number of distinct composite numbers in the n X n multiplication
 *          table.
 * https://oeis.org/A334454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A334454: Number of distinct composite numbers in the n X n
 *  multiplication table.  Formula: a(n) = |{ i*j : 2 ≤ i ≤ j ≤ n }|.
 *  The function counts distinct products i*j for all 2 ≤ i ≤ j ≤ n.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sentinel value for uncomputed cache entries (maximum unsigned value). */
#define SENTINEL ((seq_t)-1)

/* Cache array for computed a(n).  Size grows with the maximum n requested. */
static seq_t *cached = NULL;        /* Pointer to cache array.      */
static idx_t   cached_size = 0;     /* Number of elements currently allocated. */

/* Count set bits in an 8‑bit byte. */
static inline int popcount8(uint8_t b) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcount((unsigned)b);
#else
    static const uint8_t pc[256] = {
        /* 0 */0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
        /* 1 */1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        /* 2 */1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        /* 3 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        /* 4 */1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        /* 5 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        /* 6 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        /* 7 */3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        /* 8 */1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        /* 9 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       /*10 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       /*11 */3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       /*12 */2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
       /*13 */3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       /*14 */3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
       /*15 */4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
    };
    return pc[b];
#endif
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* n = 0 or 1 → answer 0 (no composite products). */
    if (n <= 1) return 0;

    /* Ensure cache is large enough. */
    if ((size_t)n >= cached_size) {
        idx_t new_size = (idx_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cached, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* Allocation failure, unlikely. */
        cached = tmp;
        for (idx_t i = cached_size; i < new_size; ++i) cached[i] = SENTINEL;
        cached_size = new_size;
    }

    /* Return cached value if already computed. */
    if (cached[n] != SENTINEL) return cached[n];

    /* Allocate a bitset capable of storing all values 0 … n*n. */
    size_t max_prod = (size_t)n * (size_t)n;           /* n² ≤ 10⁸ for n≤1e4. */
    size_t byte_cnt = (max_prod + 1 + 7) >> 3;         /* ceil((max_prod+1)/8) */
    uint8_t *bits = (uint8_t *)calloc(byte_cnt, 1);
    if (!bits) return 0;    /* Allocation failure. */

    /* Mark all products i*j with 2 ≤ i ≤ j ≤ n. */
    for (idx_t i = 2; i <= n; ++i) {
        size_t prod = (size_t)i * (size_t)i;   /* j = i */
        for (idx_t j = i; j <= n; ++j, prod += (size_t)i) {
            size_t byte_idx = prod >> 3;
            uint8_t mask = 1u << (prod & 7);
            bits[byte_idx] |= mask;
        }
    }

    /* Count set bits in the bitset. */
    idx_t count = 0;
    for (size_t k = 0; k < byte_cnt; ++k) {
        count += popcount8(bits[k]);
    }

    free(bits);                   /* Release temporary bitset. */
    cached[n] = count;            /* Store result. */
    return count;
}
