/*
 * A295404  G.f. A(x) satisfies: A(x) = A(x)^2 - x*A(x)^3 + x^2.
 * https://oeis.org/A295404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The coefficients a(n) satisfy the functional equation A(x) =
 *            A(x)^2 - x*A(x)^3 + x^2, where A(x) = Σ_{n≥0} a(n)x^n. Equating
 *            coefficients gives the recurrence a(0) = 1, a(1) = 1, for n ≥ 2:
 *            a(n) = c(n‑1) – Σ_{i=1}^{n‑1} a(i)a(n‑i) – δ_{n,2}, where c(k) =
 *            Σ_{p=0}^{k} a(p) * b(k‑p), b(k) = Σ_{i=0}^...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   allocate_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/allocate_seq_array.h"

static seq_t *a_seq = NULL;   /* a values           */
static seq_t *b_seq = NULL;   /* convolution a*a   */
static seq_t *c_seq = NULL;   /* convolution a*b   */
static size_t seq_cap = 0;    /* allocated capacity */
static idx_t  seq_len = 0;    /* highest non‑negative index computed */

/* Ensure arrays are large enough to hold indices up to ‘need’ (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)seq_cap > need) return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while ((idx_t)new_cap <= need) new_cap <<= 1;

    /* Allocate new arrays.  allocate_seq_array performs default
       construction for each element. */
    seq_t *new_a = allocate_seq_array(new_cap);
    seq_t *new_b = allocate_seq_array(new_cap);
    seq_t *new_c = allocate_seq_array(new_cap);

    /* Copy existing data to the new arrays. */
    for (idx_t i = 0; i < seq_cap; ++i) {
        new_a[i] = a_seq[i];
        new_b[i] = b_seq[i];
        new_c[i] = c_seq[i];
    }

    /* No need to free old arrays – they will remain unused until
       program termination. */

    a_seq = new_a;
    b_seq = new_b;
    c_seq = new_c;
    seq_cap = new_cap;
}

/* Compute the sequence up to index 'N' inclusive. */
static void compute_up_to(idx_t N)
{
    if (N < seq_len) return;

    ensure_capacity(N);

    /* Initialise base case n = 0. */
    if (seq_len == 0) {
        a_seq[0] = (seq_t)1;
        b_seq[0] = a_seq[0] * a_seq[0];            /* a0*a0        */
        c_seq[0] = a_seq[0] * b_seq[0];            /* a0*b0        */
        seq_len = 0;
    }

    /* Compute terms from seq_len+1 up to N. */
    for (idx_t k = seq_len + 1; k <= N; ++k) {
        /* R(k) = Σ_{i=1}^{k-1} a(i)*a(k-i) */
        seq_t R = 0;
        for (idx_t i = 1; i < k; ++i) R += a_seq[i] * a_seq[k - i];

        /* δ_{k,2} */
        seq_t delta = (k == (idx_t)2) ? (seq_t)1 : (seq_t)0;

        /* a(k) = c(k‑1) – R – δ */
        a_seq[k] = c_seq[k - 1] - R - delta;

        /* b(k) = 2*a(k) + R */
        b_seq[k] = (seq_t)2 * a_seq[k] + R;

        /* c(k) = Σ_{p=0}^{k} a(p)*b(k‑p) */
        seq_t ck = 0;
        for (idx_t p = 0; p <= k; ++p) ck += a_seq[p] * b_seq[k - p];
        c_seq[k] = ck;
    }

    seq_len = N;
}

/* External interface required by the test harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_seq[n];
}
