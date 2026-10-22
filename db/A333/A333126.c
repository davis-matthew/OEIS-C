/*
 * A333126  Numbers whose binary representation does not contain a
 *          square-subword of length 4 or more.
 * https://oeis.org/A333126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 236, u32 n <= 239, u64 n <= 239,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A333126 by exhaustive search for each integer. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
    #define CAST_SEQ(v) static_cast<seq_t>(v)
#else
    #define CAST_SEQ(v) ((seq_t)(v))
#endif

/* Convert an idx_t to seq_t. */
static inline seq_t seq_from_idx(idx_t v) {
    return CAST_SEQ(v);
}

/* Count bits of a seq_t value. */
static inline unsigned bitlen_seq_t_local(seq_t x) {
    unsigned len = 0;
    while (x != (seq_t)0) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Return true if binary representation of x has no square‑subword
   of length ≥ 4. */
static bool has_no_square(seq_t x) {
    unsigned len = bitlen_seq_t_local(x);
    if (len < 4) return true;

    for (unsigned i = 0; i + 2 <= len; ++i) {
        unsigned maxk = (len - i) / 2;
        for (unsigned k = 2; k <= maxk; ++k) {
            bool eq = true;
            for (unsigned j = 0; j < k; ++j) {
                bool b1 = ((x >> (i + j)) & ((seq_t)1)) != 0;
                bool b2 = ((x >> (i + k + j)) & ((seq_t)1)) != 0;
                if (b1 != b2) {
                    eq = false;
                    break;
                }
            }
            if (eq) return false;
        }
    }
    return true;
}

/* Cache of generated sequence terms. */
static seq_t *cache = NULL;
static size_t cache_size   = 0;   /* number of terms stored   */
static size_t cache_cap    = 0;   /* allocated slots          */
static idx_t  next_candidate = 0;  /* next integer to test    */

/* Ensure cache contains at least n terms. */
static void ensure_cache(size_t n) {
    if (n <= cache_size) return;

    while (cache_size < n) {
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            void *tmp = realloc((void*)cache, new_cap * sizeof(seq_t));
            cache = (seq_t*)tmp;
            cache_cap = new_cap;
        }

        seq_t cand = seq_from_idx(next_candidate);
        if (has_no_square(cand))
            cache[cache_size++] = cand;
        ++next_candidate;
    }
}

/* Main entry point used by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for offset 0, return 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
