/*
 * A266909  Table read by rows: for each k < n and coprime to n, the least
 *          x>=0 such that x*n+k is prime.
 * https://oeis.org/A266909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5714, u16 n <= 9050, u32 n <= 9050, u64 n <= 9050,
 *            u128 n <= 9050, big any n
 * Helpers:   coprime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A266909 – table read by rows:  for each n ≥ 2 and each
 * k < n coprime to n, return the smallest x ≥ 0 such that n·x + k is
 * prime.  The sequence is indexed starting at 1 for the first element
 * (the pair n = 2, k = 1).  The function maps an index to its
 * corresponding (n,k) pair via totient sums, then searches for the
 * desired x by trial. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t) */
#include "../utils/coprime.h"

/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
/* Storage for φ values and their cumulative sums. ------------------- */
static idx_t *phi_arr   = NULL; /* φ[i] */
static idx_t *pref_arr  = NULL; /* Σ_{j=2}^{i} φ(j) */
static idx_t  max_phi   = 0;    /* highest i for which tables are valid */

/* --------------------------------------------------------------- */
/* Build φ[0..limit] and pref[0..limit]. ----------------------------- */
static void recompute_limit(idx_t limit)
{
    idx_t *new_phi  = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    idx_t *new_pref = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_phi || !new_pref) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= limit; ++i) new_phi[i] = i;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_phi[i] == i) {                 /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                new_phi[j] -= new_phi[j] / i;
        }
    }

    new_pref[0] = new_pref[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        new_pref[i] = new_pref[i - 1] + new_phi[i];

    free(phi_arr);
    free(pref_arr);
    phi_arr   = new_phi;
    pref_arr  = new_pref;
    max_phi   = limit;
}

/* --------------------------------------------------------------- */
/* Ensure that φ and pref are computed up to an index with
 * cumulative totient sum ≥ 'need'. ----------------------------------- */
static void ensure_until(idx_t need)
{
    if (max_phi >= need) return;

    idx_t limit = max_phi < 2 ? 2 : max_phi;
    while (true) {
        if (limit < need) limit = need;
        recompute_limit(limit);
        if (pref_arr[limit] >= need) break;
        limit <<= 1;                      /* double until sufficient */
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function. ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined, guard */

    /* Build enough tables to locate the required row */
    ensure_until(n);

    /* Find the smallest row such that pref[row] ≥ n */
    idx_t row = 2;
    for (; row <= max_phi; ++row) {
        if (pref_arr[row] >= n) break;
    }

    idx_t prev_sum   = (row > 2) ? pref_arr[row - 1] : 0;
    idx_t pos_in_row = n - prev_sum - 1;     /* zero‑based index among coprimes */

    /* Locate the pos_in_row‑th coprime integer k (1 ≤ k < row) */
    idx_t k = 0;
    idx_t count = 0;
    for (k = 1; k < row; ++k) {
        if (coprime(row, k)) {
            if (count == pos_in_row) break;
            ++count;
        }
    }

    /* Search for the minimal x ≥ 0 with row·x + k prime */
    idx_t x = 0;
    for (;;) {
        seq_t candidate = (seq_t)row * (seq_t)x + (seq_t)k;
        if (is_prime_big(candidate)) break;
        ++x;
    }

    return (seq_t)x;
}
