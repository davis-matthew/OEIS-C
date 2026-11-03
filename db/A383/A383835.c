/*
 * A383835  Number of permutations of [n] whose compositional square is the
 *          identity permutation or its reverse.
 * https://oeis.org/A383835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000085(n) + A037224(n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000085(n) (involutions) satisfies:            */
/*   a(0)=1, a(1)=1, a(n)=a(n-1)+(n-1)*a(n-2)       */
/* A037224(n) counts permutations whose square is the reverse permutation. */
/* For n % 4 == 2 or 3 there are none; for n % 4 == 1 it equals the value for n-1; */
/* for n % 4 == 0: A037224(n) = 2 * Π_{j=1}^{(n-4)/4} (4*j + 2) */

/* See comments in the code for details. */

#include <stddef.h>

static seq_t involutions(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t a_prev2 = (seq_t)1;  /* a(0) */
    seq_t a_prev1 = (seq_t)1;  /* a(1) */
    seq_t a = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        a = a_prev1 + ((seq_t)(i - 1)) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a;
    }
    return a;
}

static seq_t reverse_root_counts(idx_t n) {
    if (n < 4) return (seq_t)0;
    /* If the number of 2‑cycles in the reverse permutation cannot be paired,
       there is no root. */
    if ((n & 3) == 2 || (n & 3) == 3) return (seq_t)0;
    if ((n & 1) == 1) return reverse_root_counts(n - 1);   /* same as for n-1 */

    /* n % 4 == 0: product formula. */
    idx_t m = (n - 4) / 4;          /* number of factors in product  */
    seq_t res = (seq_t)2;
    for (idx_t j = 1; j <= m; ++j) {
        res *= (seq_t)(4 * j + 2);
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t inv = involutions(n);
    seq_t rev_root = reverse_root_counts(n);
    return inv + rev_root;
}
