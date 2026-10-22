/*
 * A322024  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A014197(i) = A014197(j) and A081373(i) = A081373(j), for all i, j.
 *          Here A081373(n) gives the number of k, 1 <= k <= n, with phi(k) =
 *          phi(n), while A014197(n) gives the number of integers m with
 *          phi(m) = n.
 * https://oeis.org/A322024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1931, u16 n <= 9303, u32 n <= 9303, u64 n <= 9303,
 *            u128 n <= 9303, big any n
 * Helpers:   alloc_u64_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322024: restricted growth sequence transform of the ordered pair
   (A014197(n), A081373(n)).  For each n we compute the ordered pair
   (x = A014197(n), y = A081373(n)) and assign the lexicographically first
   integer that has not yet appeared for an identical pair.  The algorithm
   builds the sequence lazily; it recomputes all auxiliary data whenever a
   larger index is requested.  Complexity is O(n·log n) with a small
   pre‑computation of Euler's totient function. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_u64_array.h"

/* ----- internal data structures --------------------------------------- */

typedef struct {
    uint64_t x;  /* A014197(n) */
    uint64_t y;  /* A081373(n) */
} pair_t;

/* ----- static state ----------------------------------------------- */

static idx_t      max_n   = 0;          /* largest n for which a(n) is known */
static uint64_t  *a_vals  = NULL;      /* internal values of a(n) as uint64_t */
static size_t     a_cap   = 0;

static uint64_t  *freq    = NULL;      /* frequency of φ values up to current n */
static size_t     freq_cap= 0;

static uint64_t  *pre_cnt = NULL;      /* A014197: preimage counts of φ up to current n */
static size_t     pre_cap = 0;

static pair_t   *pairs   = NULL;       /* distinct pairs encountered so far */
static size_t     pair_cnt= 0;
static size_t     pair_cap= 0;

static uint64_t *phi_arr = NULL;       /* array of φ values up to a dynamic bound */
static size_t    phi_cap = 0;

/* ----- helper functions -------------------------------------------- */

/* Compute Euler's totient φ(n) for all n up to limit (inclusive) */
static void compute_phi_upto(uint64_t limit)
{
    alloc_u64_array(&phi_arr, &phi_cap, limit + 1);
    for (uint64_t i = 0; i <= limit; ++i) phi_arr[i] = i;
    if (limit >= 1) phi_arr[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (phi_arr[i] == i) {            /* i is prime */
            for (uint64_t j = i; j <= limit; j += i)
                phi_arr[j] -= phi_arr[j] / i;
        }
    }
}

/* Recompute preimage counts A014197(t) for all t <= n */
static void recompute_pre_cnt(uint64_t n)
{
    alloc_u64_array(&pre_cnt, &pre_cap, n + 1);
    for (uint64_t i = 0; i <= n; ++i) pre_cnt[i] = 0;

    /* Bound for m such that φ(m) <= n.  
       Empirically φ(m) >= m / (2 log log m) for large m, so m <= 10*n
       is comfortably safe for n <= 10000.  Add a small constant for tiny n. */
    uint64_t bound = n * 10 + 100;
    if (bound < 1000) bound = 1000;

    compute_phi_upto(bound);

    for (uint64_t m = 1; m <= bound; ++m) {
        uint64_t ph = phi_arr[m];
        if (ph <= n) pre_cnt[ph] += 1;
    }
}

/* Ensure the internal arrays for a_vals and freq can hold indices up to n */
static void ensure_arrays(idx_t n)
{
    /* a_vals: store as uint64_t then cast to seq_t on demand */
    alloc_u64_array((uint64_t**)&a_vals, &a_cap, (size_t)n);

    /* freq: one entry per possible φ value (≤ n) */
    alloc_u64_array(&freq, &freq_cap, (size_t)n + 1);
}

/* Ensure capacity for the dynamic array of pairs */
static void ensure_pairs_capacity(void)
{
    if (pair_cnt == pair_cap) {
        size_t new_cap = pair_cap == 0 ? 4 : pair_cap * 2;
        pair_t *tmp = (pair_t*)realloc(pairs, new_cap * sizeof(pair_t));
        if (!tmp) abort();          /* allocation failed */
        pairs = tmp;
        pair_cap = new_cap;
    }
}

/* ----- Main algorithm ---------------------------------------------- */

/* Extend preprocessing up to index n (inclusive) */
static void extend_to(idx_t n)
{
    if (n <= max_n) return;                 /* nothing to do */

    /* Recompute preimage counts and φ values up to the new maximum */
    recompute_pre_cnt((uint64_t)n);

    /* Ensure all auxiliary arrays can hold data up to n */
    ensure_arrays(n);
    ensure_pairs_capacity();

    /* Build the sequence for indices > max_n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        uint64_t x = pre_cnt[(uint64_t)i];                /* A014197(i) */
        uint64_t ph = phi_arr[(uint64_t)i];               /* φ(i) */
        freq[ph]++;                                       /* A081373(i) is freq[ph] */
        uint64_t y = freq[ph];

        /* Search for an existing pair */
        size_t j = 0;
        for (; j < pair_cnt; ++j) {
            if (pairs[j].x == x && pairs[j].y == y) break;
        }

        seq_t val;
        if (j == pair_cnt) {              /* new pair encountered */
            pairs[pair_cnt].x = x;
            pairs[pair_cnt].y = y;
            val = (seq_t)(pair_cnt + 1);  /* RGS numbers are 1‑based */
            pair_cnt++;
        } else {                           /* repeated pair */
            val = (seq_t)(j + 1);
        }
        a_vals[(size_t)i - 1] = (uint64_t)val;
    }

    max_n = n;
}

/* ------------------------------------------------------------------ */
/* Required function for the OEIS build system                        */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for 0, return 0 safely */
    extend_to(n);
    return (seq_t)a_vals[(size_t)n - 1];
}
