/*
 * A328557  Arithmetic numbers (A003601) that are not squares (A000290).
 * https://oeis.org/A328557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: arithmetic numbers that are not perfect squares (A328557).
   A number m is arithmetic when σ(m) (sum of divisors) is divisible
   by τ(m) (number of divisors).  This routine builds a sieve for
   σ and τ up to a growing limit until it has collected at least n
   arithmetic numbers that are not perfect squares.  The values are
   stored in a static array and are returned in order (a(1) is 3,
   a(2) is 5, ...).  The routine handles repeat calls in any order
   and works for 8‑, 16‑, 32‑, 64‑bit or arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values, used to test perfect squares. */
static inline uint64_t _floor_isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* ---------- static state ---------- */
static idx_t   cur_limit  = 0;        /* maximum m processed so far   */
static seq_t  *sigma      = NULL;     /* σ(m)   for 0..cur_limit      */
static seq_t  *tau        = NULL;     /* τ(m)   for 0..cur_limit      */
static seq_t  *terms      = NULL;     /* list of arithmetic numbers   */
static size_t  terms_cnt  = 0;        /* number of stored answers     */

/* ---------- helpers ---------- */
/* Rebuild sigma and tau arrays up to new_limit (full recomputation). */
static void recompute_arrays(idx_t new_limit)
{
    if (new_limit <= cur_limit) return;

    /* Allocate new arrays.  The cast is required for C++. */
    seq_t *new_sigma = (seq_t*)realloc(sigma, (new_limit + 1) * sizeof(seq_t));
    seq_t *new_tau   = (seq_t*)realloc(tau,   (new_limit + 1) * sizeof(seq_t));

    /* Initialise to zero. */
    for (size_t i = 0; i <= (size_t)new_limit; ++i) {
        new_sigma[i] = 0;
        new_tau[i]   = 0;
    }

    /* Compute σ and τ by a simple divisor sieve. */
    for (size_t i = 1; i <= (size_t)new_limit; ++i) {
        seq_t val_i = (seq_t)i;
        for (size_t j = i; j <= (size_t)new_limit; j += i) {
            new_sigma[j] += val_i;
            new_tau[j]   += (seq_t)1;
        }
    }

    sigma = new_sigma;
    tau   = new_tau;
    cur_limit = new_limit;
}

/* Rebuild the terms array up to a given limit. */
static void rebuild_terms_up_to(idx_t limit)
{
    recompute_arrays(limit);

    /* Allocate terms array; worst case every integer could be an entry. */
    if (terms) free(terms);
    terms = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    terms_cnt = 0;

    for (size_t m = 1; m <= (size_t)limit; ++m) {
        if (tau[m] != 0 && sigma[m] % tau[m] == 0) {          /* arithmetic */
            uint64_t r = _floor_isqrt_u64(m);
            if ((uint64_t)r * (uint64_t)r != m) {             /* not a square */
                terms[terms_cnt++] = (seq_t)m;
            }
        }
    }
}

/* Ensure that at least n terms have been computed. */
static void ensure_terms(idx_t n)
{
    if (terms_cnt >= (size_t)n) return;

    idx_t limit = cur_limit ? cur_limit : 1024;
    while (terms_cnt < (size_t)n) {
        if (limit < (idx_t)(n * 2 + 1000)) limit = (idx_t)(n * 2 + 1000);
        rebuild_terms_up_to(limit);
        limit *= 2;
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset starts at 1 */
    ensure_terms(n);
    return terms[(size_t)(n - 1)];
}
