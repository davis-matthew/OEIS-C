/*
 * A320657  a(n) is the number of non-unimodal sequences with n nonzero terms
 *          that arise as a convolution of sequences of binomial coefficients
 *          preceded by a finite number of ones.
 * https://oeis.org/A320657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+10) = * 2 * Σ_{i=1..⌊n/2⌋} floor(i*(i+4)/4) - floor(n^2/16)
 *            if n is even * 2 * Σ_{i=1..⌊(n-1)/2⌋} floor(i*(i+4)/4) -
 *            floor((n-1)^2/16) * + floor((n+1)*(n+9)/16) if n is odd * Here
 *            the sequence a(n) has offset 1; the above holds for n ≥ 0, *
 *            i.e. for a(k) with k ≥ 11
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>

/* static cache of prefix sums of floor(i*(i+4)/4) */
static seq_t *psum = NULL;      /* psum[i] = Σ_{k=1..i} floor(k*(k+4)/4) */
static idx_t psum_size = 0;     /* number of valid entries in psum */

/* Ensure that psum contains entries up to index 'm' (inclusive). */
static void ensure_psum(idx_t m)
{
    if (m < psum_size) return;          /* already available */

    idx_t new_size = m + 1;              /* we need indices 0..m */
    seq_t *new_psum = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_psum) return;               /* out of memory; caller fails */

    /* Initialize the new array.  Index 0 holds the zero sum. */
    new_psum[0] = 0;

    /* Copy existing entries, if any. */
    if (psum && psum_size > 0) {
        size_t to_copy = psum_size <= new_size ? psum_size : new_size;
        for (idx_t i = 0; i < to_copy; ++i) {
            new_psum[i] = psum[i];
        }
    }

    /* Compute new entries starting after the last old entry. */
    idx_t start = psum ? (idx_t)psum_size : 1;   /* skip 0 if not yet computed */
    for (idx_t i = start; i < new_size; ++i) {
        /* t = i*(i+4); floor division by 4 */
        idx_t t = i * (i + 4);
        seq_t add = (seq_t)(t / 4);
        new_psum[i] = new_psum[i - 1] + add;
    }

    /* Replace the old array. */
    if (psum) free(psum);
    psum = new_psum;
    psum_size = new_size;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The first ten terms are zero by convention. */
    if (n <= 10) return 0;

    /* r = n-10 corresponds to the 'n' in the OEIS formula. */
    idx_t r = n - 10;

    if (r & 1U) {  /* r is odd */
        idx_t m = (r - 1) >> 1;          /* floor((r-1)/2) */
        ensure_psum(m);
        seq_t sum = psum[m];

        idx_t r1 = r - 1;
        idx_t term1 = (r1 * r1) / 16;    /* floor((r-1)^2 / 16) */

        idx_t term2 = ((r + 1) * (r + 9)) / 16;  /* floor((r+1)*(r+9)/16) */

        return 2 * sum - (seq_t)term1 + (seq_t)term2;
    } else {        /* r is even */
        idx_t m = r >> 1;                /* r / 2 */
        ensure_psum(m);
        seq_t sum = psum[m];

        idx_t term = (r * r) / 16;       /* floor(r^2 / 16) */

        return 2 * sum - (seq_t)term;
    }
}
