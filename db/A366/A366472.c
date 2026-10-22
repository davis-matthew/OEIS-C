/*
 * A366472  Irregular triangle read by rows: T(n,k) (n >= 1, k >= 1) = number
 *          of increasing geometric progressions in {1,2,3,...,n} of length k
 *          with rational ratio.
 * https://oeis.org/A366472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the number of increasing geometric progressions in
 *            {1,…,m} of length k with rational ratio, read by rows of the
 *            triangle T(n,k) where T(n,k) = Σ_{p=2}^{⌊n^(1/(k‑1))⌋}
 *            φ(p)·⌊n/p^(k‑1)⌋; k runs from 1 to 1+⌊log2(n)⌋. The OEIS
 *            sequence A366472 is the sequence of all T(n,k) written row‑wise,
 *            a...
 * Range:     u8 n <= 90, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

static seq_t *phi = NULL;          /* array of φ values */
static size_t phi_size = 0;        /* current allocated size of phi */

/* Ensure that φ values are available up to at least limit (inclusive). */
static void ensure_phi_up_to(idx_t limit)
{
    if ((idx_t)phi_size > limit) return;  /* nothing to do */

    size_t newsize = (size_t)limit + 1;    /* include 0..limit */
    seq_t *newphi = (seq_t *)realloc(phi, newsize * sizeof(seq_t));
    if (!newphi) return;                  /* allocation failed – leave as is */

    /* initialise all entries to their index value */
    for (size_t i = 0; i < newsize; ++i)
        newphi[i] = (seq_t)i;

    /* compute φ by a simple sieve for all numbers >=2 */
    for (idx_t i = 2; i <= limit; ++i) {
        if (newphi[i] == (seq_t)i) {      /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                newphi[j] -= newphi[j] / (seq_t)i;
        }
    }

    phi     = newphi;
    phi_size = newsize;
}

/* Compute T(n,k) for given row n and column k (k >= 1). */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 1) return (seq_t)n;          /* T(n,1) = n */

    idx_t r = k - 1;                      /* exponent for p^r */
    seq_t sum = 0;

    for (idx_t p = 2; ; ++p) {
        /* compute p^r; break if it exceeds n */
        seq_t power = 1;
        for (idx_t i = 0; i < r; ++i) {
            power *= (seq_t)p;
            if (power > (seq_t)n) break;
        }
        if (power > (seq_t)n) break;

        /* make sure φ[p] is available */
        if ((idx_t)phi_size <= p) ensure_phi_up_to(p);

        /* add the contribution */
        seq_t term = phi[p] * ((seq_t)n / power);
        sum += term;
    }
    return sum;
}

/* Main sequence function: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    idx_t row = 1;          /* current row index */
    idx_t cumulative = 0;   /* number of terms up to previous row */

    while (true) {
        /* length of this row = 1 + floor(log2(row)) */
        idx_t len = 1 + floor_log2_uint64((unsigned long long)row);

        if (cumulative + len >= n) {      /* our term is in this row */
            idx_t col = n - cumulative;    /* 1‑based column within row */
            return compute_T(row, col);
        }

        cumulative += len;
        ++row;
    }
}
