/*
 * A279535  Triangle read by rows: The number of digits in the smallest
 *          9-repdigit that is a multiple of n and m, where n and m are
 *          coprime to 2 and 5.
 * https://oeis.org/A279535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = minimal d such that (A045572(i)*A045572(j)) | (10^d – 1),
 * where the index n is unfolded to triangular coordinates:
 *   n = i(i‑1)/2 + j   with 1 ≤ j ≤ i
 *
 * The implementation uses a memoised cache for the requested indices,
 * lazy evaluation of the A045572 sequence, and computes the multiplicative
 * order of 10 modulo N = A045572(i) * A045572(j).
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* ------------------------------------------------------------- */
/* Cache for values of the A045572 sequence (numbers coprime to 2 and 5) */
static seq_t *A_vals = NULL;
static idx_t  A_len  = 0;
static idx_t  A_cap  = 0;

static seq_t get_A045572(idx_t idx)
{
    /* Ensure cache contains at least idx entries */
    if ((idx_t)idx <= A_len) return A_vals[idx - 1];

    /* Allocate / grow cache if necessary */
    if (A_cap == 0) {
        A_cap = 4;
        A_vals = (seq_t *)malloc(A_cap * sizeof(seq_t));
    }
    for (idx_t i = A_len + 1; i <= idx; ++i) {
        if (A_len == 0) {
            A_vals[A_len++] = 1;
            continue;
        }
        seq_t v = A_vals[A_len - 1] + 1;
        /* Skip multiples of 2 and 5 */
        while ((v % 2 == 0) || (v % 5 == 0)) v += 1;
        if (A_len >= A_cap) {
            A_cap *= 2;
            A_vals = (seq_t *)realloc(A_vals, A_cap * sizeof(seq_t));
        }
        A_vals[A_len++] = v;
    }
    return A_vals[idx - 1];
}

/* ------------------------------------------------------------- */
/* Compute multiplicative order of 10 modulo N (N coprime to 2 and 5) */
static seq_t order_10_mod(uint64_t N)
{
    if (N == 1) return 1;

    /* Compute φ(N) via its prime factorisation */
    uint64_t primes[16], exps[16];
    size_t  pcnt = 0;
    factor_u64(N, primes, exps, &pcnt);

    uint64_t phi = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t p_pow = 1;
        for (uint64_t k = 1; k < e; ++k) p_pow *= p;      /* p^(e‑1) */
        phi *= p_pow * (p - 1);
    }

    /* Factorisation of φ to reduce the order */
    size_t   fcnt = 0;
    uint64_t fp[16], fe[16];
    factor_u64(phi, fp, fe, &fcnt);

    uint64_t d = phi;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = fp[i];
        while ((d % p) == 0) {
            seq_t val = mod_pow((seq_t)10, d / p, (seq_t)N);
            if (val == (seq_t)1) {
                d /= p;
            } else {
                break;
            }
        }
    }
    return (seq_t)d;
}

/* ------------------------------------------------------------- */
/* Cache for the sequence a(n) itself */
static seq_t *a_cache = NULL;
static idx_t  a_len   = 0;
static idx_t  a_cap   = 0;

seq_t a(idx_t n)
{
    /* 1‑based indexing: return cached value if available */
    if ((idx_t)n <= a_len && a_cache[n - 1] != (seq_t)0) {
        return a_cache[n - 1];
    }

    /* Grow cache if needed */
    if ((idx_t)n > a_cap) {
        idx_t new_cap = a_cap == 0 ? 4 : a_cap * 2;
        while ((idx_t)n > new_cap) new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
    }
    while (n > a_len) a_cache[a_len++] = (seq_t)0;  /* initialise to 0 */

    /* Unfold n into triangular coordinates (i, j) */
    idx_t i = 1;
    idx_t cum = 1;                     /* sum of first i rows */
    while (cum < n) {
        ++i;
        cum += i;
    }
    idx_t prev_sum = cum - i;          /* sum of rows before row i */
    idx_t j = n - prev_sum;            /* 1‑based column */

    seq_t vi = get_A045572(i);
    seq_t vj = get_A045572(j);
    uint64_t N = (uint64_t)vi * (uint64_t)vj;   /* safe: values < 250 */

    seq_t d = order_10_mod(N);
    a_cache[n - 1] = d;
    return d;
}
