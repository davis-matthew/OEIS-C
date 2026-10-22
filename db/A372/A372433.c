/*
 * A372433  Binary weight (number of ones in binary expansion) of the n-th
 *          squarefree number.
 * https://oeis.org/A372433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(A005117(n)) // binary weight of the n‑th
 *            squarefree number
 * Range:     u8 n <= 197, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 *  Global state used for memoisation
 * ------------------------------------------------------------------ */
static seq_t *sqfree = NULL;          /* stored squarefree numbers */
static size_t sq_cap   = 0;           /* allocated capacity for sqfree */
static size_t sq_len   = 0;           /* number of stored squarefree numbers */

static uint64_t *spf   = NULL;        /* smallest prime factor table */
static size_t spf_cap  = 0;           /* allocated capacity for spf */
static size_t spf_lim  = 0;           /* highest integer whose SPF is known */

static uint64_t current_test = 1;     /* next integer to be tested for squarefreeness */

/* ------------------------------------------------------------------
 *  Build a smallest‑prime‑factor sieve up to LIMIT (inclusive).
 * ------------------------------------------------------------------ */
static void build_spf(size_t limit)
{
    /* free any previous array */
    if (spf) {
        free(spf);
        spf = NULL;
    }

    spf_cap = limit + 1;
    spf = (uint64_t *)malloc(spf_cap * sizeof(uint64_t));
    if (!spf) return;                /* allocation failed – unlikely */

    for (size_t i = 0; i <= limit; ++i) spf[i] = i;
    for (size_t i = 2; i * i <= limit; ++i)
        if (spf[i] == i) {
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }

    spf_lim = limit;
}

/* ------------------------------------------------------------------
 *  Ensure that the SPF sieve covers at least N.
 * ------------------------------------------------------------------ */
static void ensure_spf_for(uint64_t N)
{
    if (N <= spf_lim) return;
    size_t new_limit = spf_lim ? spf_lim * 2 : 128;
    while (new_limit < N) new_limit *= 2;
    build_spf(new_limit);
}

/* ------------------------------------------------------------------
 *  Test whether an unsigned 64‑bit integer is square‑free.
 *  The test uses the SPF sieve.
 * ------------------------------------------------------------------ */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x <= 1) return true;
    while (x > 1) {
        uint64_t p = spf[x];
        x /= p;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------
 *  Ensure that the array of squarefree numbers has enough capacity.
 * ------------------------------------------------------------------ */
static void ensure_sqfree_capacity(size_t needed)
{
    if (needed <= sq_cap) return;
    size_t new_cap = sq_cap ? sq_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(sqfree, new_cap * sizeof(seq_t));
    if (!tmp) return;                /* allocation failed – unlikely in tests */
    sqfree = tmp;
    sq_cap = new_cap;
}

/* ------------------------------------------------------------------
 *  Count 1‑bits in an arbitrary precision seq_t value.
 *  Works for all defined seq_t types that provide bitwise operators.
 * ------------------------------------------------------------------ */
static seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;                    /* clear lowest set bit */
    }
    return cnt;
}

/* ------------------------------------------------------------------
 *  Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* OEIS offset is 1 */

    /* Extend the list of squarefree numbers until we have at least n of them. */
    while (sq_len < (size_t)n) {
        ensure_spf_for(current_test);
        if (is_squarefree_uint64(current_test)) {
            ensure_sqfree_capacity(sq_len + 1);
            sqfree[sq_len++] = (seq_t)current_test;
        }
        current_test++;                /* next integer to test */
    }

    /* Return the binary weight of the n‑th squarefree number. */
    return popcount_seq(sqfree[n - 1]);
}
