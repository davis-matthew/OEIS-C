/*
 * A277719  Index for the bound for the first k-Ramanujan prime.
 * https://oeis.org/A277719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index h(n) of the nth largest prime ratio
 *            p_i/p_{i-1}. The sequence is computed by generating a list of
 *            primes, building an array of suffix maxima of the ratios, and
 *            then extracting the indices of the successive maxima in
 *            decreasing order
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Prime storage (zero‑based indices). */
static uint64_t *primes      = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   primes_cnt   = 0;      /* number of primes stored           */

/* For each position i (1‑based ratio index), suffix_max[i] contains the
   zero‑based index of the prime that gives the maximum ratio among
   indices >= i. */
static idx_t   *suffix_max   = NULL;

/* Cached values of the OEIS sequence (1‑based output). */
static idx_t   *seq_arr     = NULL;     /* array of a(n) values          */
static size_t   seq_len      = 0;       /* number of terms currently stored   */
static size_t   seq_cap      = 0;       /* allocated capacity                 */
static idx_t   last_zero_pos = 0;       /* zero‑based index of last selected ratio */

/* ------------------------------------------------------------------
   Comparer for two ratios p_a/p_{a-1} and p_b/p_{b-1}.  Returns true
   if the first ratio is strictly larger than the second one.
   ------------------------------------------------------------------ */
static inline bool ratio_greater(idx_t a, idx_t b)
{
    /* a, b >= 1 (zero‑based indices with valid a-1, b-1).    */
    unsigned __int128 left  = (unsigned __int128)primes[a] * primes[b - 1];
    unsigned __int128 right = (unsigned __int128)primes[b] * primes[a - 1];
    return left > right;
}

/* ------------------------------------------------------------------
   Build the suffix_max array.
   ------------------------------------------------------------------ */
static void compute_suffix_max(void)
{
    if (primes_cnt < 2) return;            /* not enough primes */

    suffix_max[primes_cnt - 1] = primes_cnt - 1;
    for (idx_t i = primes_cnt - 2; i >= 1; --i) {
        idx_t cand = i;
        idx_t best = suffix_max[i + 1];
        suffix_max[i] = ratio_greater(cand, best) ? cand : best;
        if (i == 1) break;                 /* avoid underflow    */
    }
}

/* ------------------------------------------------------------------
   Grow the seq_arr buffer to at least new_cap elements.
   ------------------------------------------------------------------ */
static void grow_seq(size_t new_cap)
{
    seq_arr = (idx_t *)realloc(seq_arr, new_cap * sizeof(idx_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------
   One‑time initialization: primes, suffix_max, and first sequence value.
   ------------------------------------------------------------------ */
static void init(void)
{
    if (primes != NULL) return;            /* already done */

    /* A limit that guarantees many primes (approximately 300 000). */
    const idx_t PRIME_LIMIT = 4000000ULL;   /* 4 million */

    build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);

    suffix_max = (idx_t *)malloc(primes_cnt * sizeof(idx_t));
    compute_suffix_max();

    seq_cap  = 128;
    seq_arr  = (idx_t *)malloc(seq_cap * sizeof(idx_t));
    seq_len  = 0;

    /* The first term (h(1)) is the index of the largest ratio. */
    last_zero_pos = suffix_max[1];          /* zero‑based index   */
    seq_arr[seq_len++] = last_zero_pos + 1; /* store as 1‑based   */
}

/* ------------------------------------------------------------------
   Return the OEIS sequence value a(n) for any n >= 1.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* defensive for n=0 */

    init();

    /* Generate terms on demand. */
    while (seq_len < (size_t)n) {
        idx_t next_zero = suffix_max[last_zero_pos + 1];  /* next ratio index */

        if (seq_len == seq_cap)
            grow_seq(seq_cap * 2 + 64);

        seq_arr[seq_len++] = next_zero + 1;        /* store 1‑based index */
        last_zero_pos = next_zero;
    }

    return (seq_t)seq_arr[n - 1];
}
