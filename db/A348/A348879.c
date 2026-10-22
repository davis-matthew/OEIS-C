/*
 * A348879  G.f. A(x) satisfies: A(x) = 1 / (1 - x - x^2 * A(3*x)).
 * https://oeis.org/A348879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348879
   Recurrence: a(0) = 1
                a(n) = a(n-1) + Σ_{k=0}^{n-2} 3^k * a(k) * a(n-k-2)
   (Compute from n=0 upwards, cache previously computed values for random order
   access.)
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static caches for already computed terms and powers of 3 */
    static seq_t *seq = NULL;         /* a[0..cur_max] */
    static seq_t *pow3 = NULL;        /* pow3[k] = 3^k  for k=0..n-1 */
    static size_t seq_cap = 0;        /* capacity of seq   */
    static size_t pow3_cap = 0;       /* capacity of pow3  */
    static size_t cur_max = 0;        /* highest index computed (-1 indicates none) */

    size_t nn = (size_t)n;            /* safe cast, n is never > SIZE_MAX in tests */

    /*--- ensure seq has room up to n ----------------------------------*/
    if (nn + 1 > seq_cap) {
        size_t new_cap = nn + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /*--- ensure powers of 3 up to 3^(n-1) --------------------------------*/
    if (nn > pow3_cap) {               /* need pow3[n-1] when nn>=1 */
        size_t new_cap = nn;           /* because we need indices 0..nn-1 */
        pow3 = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        if (pow3_cap == 0) {           /* first time, initialise */
            pow3[0] = 1;
            pow3_cap = 1;
        }
        for (size_t i = pow3_cap; i < new_cap; ++i)
            pow3[i] = pow3[i - 1] * 3;
        pow3_cap = new_cap;
    }

    /*--- initialise a[0] and a[1] if not already done ---------------------*/
    if (cur_max < 1) {
        seq[0] = 1;
        if (nn == 0) return seq[0];
        seq[1] = seq[0]; /* a(1) = a(0) */
        cur_max = 1;
    }

    /*--- compute terms from cur_max+1 up to n ---------------------------*/
    for (size_t i = cur_max + 1; i <= nn; ++i) {
        seq_t val = seq[i - 1]; /* start with a(i-1) */
        if (i >= 2) {
            for (size_t k = 0; k <= i - 2; ++k)
                val += pow3[k] * seq[k] * seq[i - k - 2];
        }
        seq[i] = val;
    }
    cur_max = nn;
    return seq[n];
}
