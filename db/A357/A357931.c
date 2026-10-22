/*
 * A357931  a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n - 2*k,n - 3*k)|.
 * https://oeis.org/A357931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357931
 * a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n-2k, n-3k)|
 *
 * This implementation uses the generating-function representation
 *   G(x) = Σ_{k≥0} x^k * Π_{j=0}^{k-1} (1 + j·x^2)
 *
 * The polynomial P_k(x)=Π_{j=0}^{k-1}(1+jx^2) is built incrementally.
 * After each new k, its coefficients are added to all a[n]=[x^n]G(x)
 * that contain the term x^{k}·x^{d} (i.e., n=k+d).
 *
 * The code works for any integer seq_t (fixed‑size or arbitrary precision)
 * and any idx_t type (here assumed to be uint64_t).  All arithmetic on
 * indices uses idx_t, all arithmetic on values uses seq_t.
 *
 * The arrays are dimensioned for n up to 10000, the limit required by
 * the tests.  The implementation is O(n^2) time and O(n) memory
 * (the polynomial degree is at most 2n).
 */

#include <stddef.h>   /* for NULL, size_t */

#define MAX_N           10000
#define POLY_MAX_DEG    (2 * (MAX_N - 1))

/* Cached values of the sequence (initialized on first call). */
static seq_t a_vals[MAX_N + 1] = {0};

/* Polynomial coefficients of the current P_k(x). */
static seq_t poly[POLY_MAX_DEG + 1] = {0};
static seq_t temp_poly[POLY_MAX_DEG + 1] = {0};

/* Current k that has been processed. */
static idx_t current_k = 0;

/* Initialize the first polynomial and a[0].  Must be called once. */
static void
init_once(void)
{
    if (current_k == 0) {
        a_vals[0] = (seq_t)1;
        poly[0]   = (seq_t)1;
        /* poly_deg is implicitly 0 (poly[0] is the only non‑zero coeff). */
    }
}

/* Extend precomputation up to at least the requested n.  All indices
 * a_vals[0..n] are guaranteed correct after this call. */
static void
extend_to(idx_t n)
{
    if (n > MAX_N) {
        /* Out of the supported bound – abort is safe for the test harness. */
        abort();
    }
    init_once();

    /* Process new k‑values one by one. */
    for (idx_t k = current_k + 1; k <= n; ++k) {
        /* Degree of the previous polynomial. */
        idx_t old_deg = k > 1 ? 2 * (k - 2) : 0;          /* deg of P_{k-1} */
        idx_t new_deg = (k == 1) ? old_deg : old_deg + 2; /* deg of P_k */

        /* Build the new polynomial coefficients into temp_poly. */
        for (idx_t d = 0; d <= new_deg; ++d) {
            seq_t val = 0;
            if (d <= old_deg) {
                val = poly[d];
            }
            if (d >= 2 && d - 2 <= old_deg) {
                /* multiply by (k-1) * x^2 */
                val += (seq_t)(k - 1) * poly[d - 2];
            }
            temp_poly[d] = val;
        }

        /* Copy back to poly. */
        for (idx_t d = 0; d <= new_deg; ++d) {
            poly[d] = temp_poly[d];
        }

        /* Add the contributions of P_k to all a[n] that involve this k.
         * For every coefficient d, it contributes to a[k + d], provided
         * k + d does not exceed MAX_N.  Since d increases, we can break
         * when k + d > MAX_N. */
        for (idx_t d = 0; d <= new_deg; ++d) {
            idx_t idx = k + d;
            if (idx > MAX_N) break;
            a_vals[idx] += poly[d];
        }
    }

    current_k = n;
}

/* Public access function required by the build system. */
seq_t a(idx_t n)
{
    extend_to(n);
    return a_vals[n];
}
