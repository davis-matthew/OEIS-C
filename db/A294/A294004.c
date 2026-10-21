/*
 * A294004  Number of multisets of exactly two nonempty words with a total of
 *          n letters over n-ary alphabet such that within each prefix of a
 *          word every letter of the alphabet is at least as frequent as the
 *          subsequent alphabet letter.
 * https://oeis.org/A294004
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^2] Π_{j≥1} 1/(1 - y*x^j)^A000085(j)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to: */
/*   a(n) = Σ_{1 ≤ j < k, j + k = n} A000085(j)·A000085(k)   */
/*            + δ_{n even} · (A000085(n/2)·(A000085(n/2)+1))/2   */
/* with A000085(0)=1, A000085(1)=1, and for m≥2: */
/*   A000085(m) = A000085(m-1) + (m-1)*A000085(m-2).          */

#include <stdlib.h>
#include <stdbool.h>

/* static cache for involution counts */
static seq_t *invol = NULL;
static idx_t invol_current = 0;   /* highest index computed so far   */
static size_t invol_cap = 0;      /* allocated capacity               */

static void ensure_invol(idx_t n) {
    if (n <= invol_current)
        return;

    /* initial allocation */
    if (!invol) {
        invol_cap = 2;
        invol = (seq_t *)malloc(invol_cap * sizeof(seq_t));
        invol[0] = (seq_t)1;
        invol[1] = (seq_t)1;
        invol_current = 1;
    }

    /* grow capacity if necessary */
    if ((size_t)n + 1 > invol_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(invol, new_cap * sizeof(seq_t));
        invol = tmp;
        invol_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = invol_current + 1; i <= n; ++i) {
        invol[i] = invol[i - 1] + (seq_t)(i - 1) * invol[i - 2];
    }
    invol_current = n;
}

seq_t a(idx_t n) {
    if (n < 2)   /* offset is 2; below that behaviour is unspecified */
        return (seq_t)0;

    ensure_invol(n);

    seq_t total = 0;
    idx_t limit = (n - 1) / 2;

    /* sum over pairs j < k with j + k = n */
    for (idx_t j = 1; j <= limit; ++j) {
        total += invol[j] * invol[n - j];
    }

    /* add the j == k (n even) term */
    if ((n & 1) == 0) {
        idx_t j = n / 2;
        seq_t val = invol[j];
        total += val * (val + (seq_t)1) / (seq_t)2;
    }

    return total;
}
