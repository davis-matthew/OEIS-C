/*
 * A246610  Number of endofunctions on [n] whose cycle lengths are multiples
 *          of 3.
 * https://oeis.org/A246610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246610
 * Number of endofunctions on [n] whose cycle lengths are multiples of 3.
 * Formula:  a(n) = Σ_{r=3,6, … ,n} binom(n,r) * p(r) * r * n^(n-r-1),
 *           where p(r) = number of permutations of r elements with all cycle lengths
 *           divisible by 3.  For r = n the factor r·n^(n-r-1) equals 1,
 *           so the summand reduces to binom(n,n)·p(n) = p(n).
 *
 * The implementation uses dynamic programming to compute the values
 * of p(r) and factorials up to the maximum n requested.  All
 * arithmetic is performed with the arbitrary‑precision type seq_t.
 *
 * The function is safe to call with indices in any order and is
 * memoised for all previously computed values.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

/* generous maximum for n (problem limits about 10000) */
#define MAX_N 20000

static seq_t p[MAX_N + 1];           /* p[r]  – permutations of r with cycle lengths %3==0 */
static seq_t fact_tbl[MAX_N + 1];    /* factorial table             */
static bool initialized = false;
static idx_t max_r_compiled = 0;     /* highest r for which p[r] is known */
static idx_t max_fact_compiled = 0;  /* highest factorial computed    */

/* ensure tables are large enough for the requested n */
static void ensure_capacity(idx_t n) {
    if (!initialized) {
        /* initialise tables */
        fact_tbl[0] = 1;
        p[0] = 1;      /* conventionally 1 for empty permutation */
        initialized = true;
    }
    if (n > MAX_N) {
        /* the static arrays are too small – abort in a safe way */
        /* you may replace this with dynamic allocation if required */
        return;
    }

    /* extend factorial table */
    for (idx_t i = max_fact_compiled + 1; i <= n; ++i) {
        fact_tbl[i] = fact_tbl[i - 1] * (seq_t)i;
    }
    max_fact_compiled = n;

    /* extend p table */
    for (idx_t r = max_r_compiled + 1; r <= n; ++r) {
        if (r < 3) {
            p[r] = 0;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 3; k <= r; k += 3) {
            seq_t temp = binom(r - 1, k - 1);      /* choose the other k-1 elements of the cycle */
            temp = temp * fact_tbl[k - 1];        /* arrange them into a cycle of length k */
            temp = temp * p[r - k];               /* remaining elements */
            sum = sum + temp;
        }
        p[r] = sum;
    }
    max_r_compiled = n;
}

/* ------------------------------------------------------------- */
/*  Main sequence function                                       */
/* ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 1;                 /* a(0) = 1 by definition */

    if (n > MAX_N) {
        /* out of bounds – the static arrays cannot handle it.
           The problem statement guarantees n ≤ ~10000. */
        return 0;
    }

    ensure_capacity(n);

    seq_t total = 0;
    /* iterate over possible numbers of cycle nodes: r = 3,6,...,n */
    for (idx_t r = 3; r <= n; r += 3) {
        seq_t comb = binom(n, r);         /* choose the r nodes that will be on cycles      */
        seq_t term = comb * p[r];         /* add factor that counts permutations of the cycle nodes */

        if (r < n) {
            /* multiply by r and by n^(n-r-1) */
            term = term * (seq_t)r;
            seq_t pow_val = big_pow((seq_t)n, (seq_t)(n - r - 1));
            term = term * pow_val;
        }
        /* r == n : n^(n-r-1) = n^(-1) and r * n^(n-r-1) = 1; term already correct */
        total = total + term;
    }

    return total;
}
