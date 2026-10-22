/*
 * A320650  Expansion of 1/(1 - Sum_{k>=1} x^k/(1 - x^(2*k))).
 * https://oeis.org/A320650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} A001227(k)*a(n-k), a(0)=1 where A001227(k) =
 *            number of odd divisors of k
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* External types provided by the build system.  Do not redefining them. */
extern seq_t a(idx_t n);  /* for completeness */

/* ---- Private state --------------------------------------------------- */
static seq_t *a_seq  = NULL;  /* array of a-values */
static size_t a_cap  = 0;     /* allocated capacity */
static size_t a_len  = 0;     /* number of computed values */

static size_t *odddiv = NULL; /* A001227 values */
static size_t odd_cap  = 0;

static int   *spf     = NULL; /* smallest prime factor table */
static size_t spf_cap = 0;

/* ---- Utility functions ---------------------------------------------- */

/* Ensure that a_seq has capacity for at least sz elements. */
static void ensure_a_capacity(size_t sz)
{
    if (sz <= a_cap) return;
    a_seq = seq_realloc(a_seq, a_cap, sz);
    a_cap = sz;
}

/* Ensure that odddiv has capacity for at least sz elements. */
static void ensure_odd_capacity(size_t sz)
{
    if (sz <= odd_cap) return;
    odddiv = (size_t *)realloc(odddiv, sz * sizeof(size_t));
    odd_cap = sz;
}

/* Ensure that spf has capacity for at least sz elements. */
static void ensure_spf_capacity(size_t sz)
{
    if (sz <= spf_cap) return;
    spf = (int *)realloc(spf, sz * sizeof(int));
    spf_cap = sz;
}

/* Build smallest‑prime‑factor table up to N (inclusive). */
static void build_spf(size_t N)
{
    ensure_spf_capacity(N + 1);
    for (size_t i = 0; i <= N; ++i) spf[i] = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (i * i <= N) {
                for (size_t j = i * i; j <= N; j += i)
                    if (spf[j] == 0) spf[j] = (int)i;
            }
        }
    }
}

/* Compute A001227(k) for all k ≤ N. */
static void build_odddiv(size_t N)
{
    ensure_odd_capacity(N + 1);
    odddiv[0] = 0;                     /* unused */
    for (size_t k = 1; k <= N; ++k) {
        size_t m  = k;
        while ((m & 1u) == 0) m >>= 1; /* strip factors of 2 */
        size_t cnt = 1;
        while (m > 1) {
            int p   = spf[m];
            size_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            cnt *= (size_t)(e + 1);
        }
        odddiv[k] = cnt;
    }
}

/* ---- Main routine ----------------------------------------------------- */

/* Ensure that all tables contain values up to n (inclusive). */
static void ensure_a(idx_t n)
{
    size_t N = (size_t)n;
    if (N < a_len) return;

    /* Prepare tables.  We grow them in one step to simplify. */
    ensure_a_capacity(N + 1);
    if (spf_cap <= N) build_spf(N);
    if (odd_cap <= N) build_odddiv(N);

    /* Compute a(n) values from the first missing index up to N. */
    if (a_len == 0) {
        a_seq[0] = (seq_t)1;      /* a(0) = 1 */
        a_len   = 1;
    }

    for (size_t i = a_len; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= i; ++k) {
            sum += (seq_t)(odddiv[k]) * a_seq[i - k];
        }
        a_seq[i] = sum;
    }
    a_len = N + 1;
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_seq[(size_t)n];
}
