/*
 * A394928  Number of endofunctions f:{1,2,...,n}-> {1,2,...,n} whose smallest
 *          weakly connected component has exactly 2 nodes.
 * https://oeis.org/A394928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f1[n] – f12[n] where f1[n] = Σ_{k=2}^n C(n‑1,k‑1) · c[k]
 *            · f1[n‑k] f12[n] = Σ_{k=3}^n C(n‑1,k‑1) · c[k] · f12[n‑k] and
 *            the connected endofunctions are c[n] = n^n – Σ_{k=1}^{n‑1}
 *            C(n‑1,k‑1) · c[k] · (n‑k)^(n‑k) . All arithmetic is exact and
 *            uses only integer operations
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"    /* binomial(idx_t n, idx_t k) -> seq_t */
#include "../utils/pow_uint.h"    /* pow_uint(uint64_t base, idx_t exp) -> seq_t */

static seq_t *connected = NULL;  /* c[n]  */
static seq_t *f1_cache  = NULL;  /* f1[n] */
static seq_t *f12_cache = NULL;  /* f12[n] */
static seq_t *ftotal    = NULL;  /* n^n  */
static size_t cap = 0;           /* allocated capacity (>= 1) */
static idx_t last_computed = 0;  /* highest n already computed */

/* Ensure arrays are allocated with at least (need+1) positions. */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)cap > need) { last_computed = last_computed; return; }

    size_t new_cap = (size_t)need + 1;
    if (new_cap < 2 * cap) new_cap = 2 * cap;
    if (new_cap == 0) new_cap = 1;

    connected = (seq_t*)realloc(connected, new_cap * sizeof(seq_t));
    f1_cache  = (seq_t*)realloc(f1_cache,  new_cap * sizeof(seq_t));
    f12_cache = (seq_t*)realloc(f12_cache, new_cap * sizeof(seq_t));
    ftotal    = (seq_t*)realloc(ftotal,    new_cap * sizeof(seq_t));

    /* Initialise the new slots to zero to avoid garbage. */
    for (size_t i = cap; i < new_cap; ++i) {
        connected[i] = 0;
        f1_cache[i]  = 0;
        f12_cache[i] = 0;
        ftotal[i]    = 0;
    }
    cap = new_cap;
}

/* Initialise the base cases if this is the first call. */
static void init_base(void)
{
    if (connected == NULL) {
        /* Allocate for at least one element. */
        cap = 1;
        connected = (seq_t*)malloc(cap * sizeof(seq_t));
        f1_cache  = (seq_t*)malloc(cap * sizeof(seq_t));
        f12_cache = (seq_t*)malloc(cap * sizeof(seq_t));
        ftotal    = (seq_t*)malloc(cap * sizeof(seq_t));

        /* Base values for n = 0. */
        connected[0] = 0;  /* no connected endofunction on 0 nodes */
        f1_cache[0]  = 1;  /* empty function satisfies “no component of size 1” */
        f12_cache[0] = 1;  /* empty function satisfies “no component of size 1 or 2” */
        ftotal[0]    = 1;  /* 0^0 defined as 1 */
        last_computed = 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;

    init_base();
    ensure_capacity(n);

    /* Compute all values up to n that have not been computed yet. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* ftotal[i] = i^i */
        seq_t pow_ii = pow_uint((uint64_t)i, i);
        ftotal[(size_t)i] = pow_ii;

        /* connected[i] = i^i – Σ_{k=1}^{i-1} C(i-1,k-1)*connected[k]*ftotal[i-k] */
        seq_t sum_conn = 0;
        for (idx_t k = 1; k < i; ++k) {
            seq_t bin = binomial(i - 1, k - 1);
            sum_conn += bin * connected[(size_t)k] * ftotal[(size_t)(i - k)];
        }
        connected[(size_t)i] = ftotal[(size_t)i] - sum_conn;

        /* f1[i] = Σ_{k=2}^{i} C(i-1,k-1)*connected[k]*f1[i-k] */
        seq_t sum_f1 = 0;
        for (idx_t k = 2; k <= i; ++k) {
            seq_t bin = binomial(i - 1, k - 1);
            sum_f1 += bin * connected[(size_t)k] * f1_cache[(size_t)(i - k)];
        }
        f1_cache[(size_t)i] = sum_f1;

        /* f12[i] = Σ_{k=3}^{i} C(i-1,k-1)*connected[k]*f12[i-k] */
        seq_t sum_f12 = 0;
        for (idx_t k = 3; k <= i; ++k) {
            seq_t bin = binomial(i - 1, k - 1);
            sum_f12 += bin * connected[(size_t)k] * f12_cache[(size_t)(i - k)];
        }
        f12_cache[(size_t)i] = sum_f12;
    }

    last_computed = n;

    /* a(n) = f1[n] – f12[n]  (always non‑negative for the sequence) */
    return f1_cache[(size_t)n] - f12_cache[(size_t)n];
}
