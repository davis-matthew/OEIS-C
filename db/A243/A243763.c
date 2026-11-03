/*
 * A243763  Expansion of q * phi(q)^3 * psi(q^2)^4 in powers of q where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A243763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 936 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A243763
 *  a(n) = coefficient of q^n in q * phi(q)^3 * psi(q^2)^4
 *
 *  The code builds the polynomials phi(q) and psi(q^2) up to the
 *  required power and convolves them to obtain the generating
 *  function coefficient.  It keeps the arrays in static storage and
 *  expands them only when a larger index is requested.
 *
 *  The function has external linkage:
 *      seq_t a(idx_t n)
 *
 *  seq_t and idx_t are defined by the build system; no type
 *  redefinition is performed here.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/mem.h"

/* convolution of a[0..asz-1] with b[0..bsz-1] into res[0..rsz-1] */
static inline void conv_seq(const seq_t *a, size_t asz,
                            const seq_t *b, size_t bsz,
                            seq_t *res, size_t rsz)
{
    for (size_t i = 0; i < asz; ++i) {
        seq_t ai = a[i];
        if (!ai) continue;
        size_t maxj = (i + bsz > rsz) ? (rsz > i ? rsz - i : 0) : bsz;
        for (size_t j = 0; j < maxj; ++j) {
            res[i + j] += ai * b[j];
        }
    }
}

static seq_t *phi_arr   = NULL;
static seq_t *psi_arr   = NULL;
static seq_t *phi3_arr = NULL;
static seq_t *psi4_arr = NULL;
static seq_t *g_arr    = NULL;
static idx_t  cached_n = 0;

/* extend all tables up to index n (inclusive) */
static void build_up_to(idx_t n)
{
    if (cached_n >= n) return;
    size_t sz = (size_t)n + 1;

    /* free previous data */
    mem_free(phi_arr);  mem_free(psi_arr);
    mem_free(phi3_arr); mem_free(psi4_arr);
    mem_free(g_arr);

    /* allocate fresh arrays */
    phi_arr   = mem_new(seq_t, sz);
    psi_arr   = mem_new(seq_t, sz);
    phi3_arr = mem_new(seq_t, sz);
    psi4_arr = mem_new(seq_t, sz);
    g_arr    = mem_new(seq_t, sz);

    /* phi(q) = 1 + 2∑_{k≥1} q^{k^2} */
    phi_arr[0] = 1;
    for (size_t k = 1; k * k <= n; ++k) {
        phi_arr[k * k] += 2;      /* coefficient for q^{k^2} */
    }

    /* psi(q^2) = ∑_{m≥0} q^{m(m+1)} */
    psi_arr[0] = 1;
    for (size_t m = 1;; ++m) {
        size_t t = m * (m + 1);
        if (t > n) break;
        psi_arr[t] += 1;
    }

    /* phi^3 = phi * phi * phi */
    seq_t *tmp = mem_new(seq_t, sz);
    conv_seq(phi_arr, sz, phi_arr, sz, tmp, sz);     /* phi^2 */
    conv_seq(tmp, sz, phi_arr, sz, phi3_arr, sz);    /* phi^3 */
    mem_free(tmp);

    /* psi^4 = (psi^2)^2 */
    tmp = mem_new(seq_t, sz);
    conv_seq(psi_arr, sz, psi_arr, sz, tmp, sz);     /* psi^2 */
    conv_seq(tmp, sz, tmp, sz, psi4_arr, sz);        /* psi^4 */
    mem_free(tmp);

    /* G = phi^3 * psi^4 */
    conv_seq(phi3_arr, sz, psi4_arr, sz, g_arr, sz);

    cached_n = n;
}

/* external sequence function requested by the problem */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1; return 0 */
    build_up_to(n);
    /* a(n) = coefficient of q^{n-1} in phi^3 * psi^4 */
    return g_arr[(size_t)(n - 1)];
}
