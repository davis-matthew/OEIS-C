/*
 * A372538  Numbers k such that the number of ones minus the number of zeros
 *          in the binary expansion of the k-th prime number is 1.
 * https://oeis.org/A372538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k such that the (k‑th) prime has *
 *            (number of 1's in its binary expansion) – (number of 0's) == 1
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes_upto, seq_free, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the following utility headers:
 *   "../utils/build_primes_upto.h"
 *   "../utils/seq_realloc.h"
 *   "../utils/seq_free.h"
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_free.h"

/* Static storage for primes and the sequence */
static uint64_t *primes = NULL;          /* list of primes up to prime_limit */
static size_t     primes_cnt = 0;        /* number of primes stored */
static idx_t      prime_limit = 0;       /* current maximum prime value */

static seq_t *seq = NULL;                /* array of sequence values (indices k) */
static size_t seq_len = 0;               /* number of values stored */
static size_t seq_cap = 0;               /* capacity of seq array */

/* Push a value onto the sequence array, reallocating if necessary */
static void seq_push(idx_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)val;
}

/* Recompute the whole sequence from scratch using the current primes list */
static void recompute_sequence(void) {
    if (seq) seq_free(seq);
    seq = NULL;
    seq_len = 0;
    seq_cap = 0;

    for (idx_t i = 1; i <= primes_cnt; ++i) {
        uint32_t p = (uint32_t)primes[i - 1];          /* prime value */
        unsigned ones  = __builtin_popcount(p);         /* number of 1's */
        unsigned len   = 32 - __builtin_clz(p);          /* binary length */
        unsigned zeros = len - ones;                    /* number of 0's */
        if (ones - zeros == 1)                          /* condition */
            seq_push(i);                                 /* record index i */
    }
}

/* Ensure primes up to at least 'limit' are available. */
static void ensure_primes_limit(idx_t limit) {
    if (limit <= prime_limit) return;

    uint64_t *newp;
    size_t newcnt;
    build_primes_upto(limit, &newp, &newcnt);

    free(primes);
    primes    = newp;
    primes_cnt = newcnt;
    prime_limit = limit;
}

/* Main function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* offset starts at 1; return 0 for safety */

    /* Generate sequence values as needed, extending the primes list if required. */
    while (seq_len < (size_t)n) {
        idx_t new_limit = prime_limit ? prime_limit * 2 : 100000;
        ensure_primes_limit(new_limit);
        recompute_sequence();
        /* If even after expanding the limit we still lack enough values,
           the next loop will double the limit again. */
    }
    return seq[n - 1];                   /* zero‑based array access */
}
