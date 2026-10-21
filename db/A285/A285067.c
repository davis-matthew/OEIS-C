/*
 * A285067  Row sums of triangle A285066.
 * https://oeis.org/A285067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 4^k * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial value: a(0) = 1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_seq.h"

static seq_t *A          = NULL;   /* cache for a(n) values */
static size_t A_cap      = 0;
static size_t A_len      = 0;      /* number of computed values */

static seq_t *P4         = NULL;   /* powers of 4: P4[k] = 4^k */
static size_t P4_cap     = 0;
static size_t P4_len     = 0;

/* Ensure P4[] contains powers up to k = n */
static void ensure_pow4(idx_t n)
{
    if (P4_len > n) return;
    grow_seq(&P4, &P4_cap, (size_t)(n + 1));

    if (P4_len == 0) {
        P4[0] = 1;
        P4_len = 1;
    }
    for (idx_t i = P4_len; i <= n; ++i) {
        P4[i] = P4[i - 1] * 4;      /* mul_small by 4 */
    }
    P4_len = (size_t)(n + 1);
}

/* Compute a(n) (and all lower values) and store in A[] */
static void ensure_A(idx_t n)
{
    if (A_len > n) return;

    grow_seq(&A, &A_cap, (size_t)(n + 1));

    for (idx_t i = A_len; i <= n; ++i) {
        /* ensure powers of 4 up to i are ready */
        ensure_pow4(i);

        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(n,0) = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* binom = binom(n, k) progressively */
            binom = binom * (unsigned)(i - k + 1) / (unsigned)k;

            seq_t term = binom * P4[k];
            term = term * A[i - k];
            sum += term;
        }
        sum += 1;  /* add the constant 1 from the recurrence */

        A[i] = sum;
    }
    A_len = (size_t)(n + 1);
}

/* External function to be called for any n (0 <= n) */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1) { /* defensive check for unsigned wrap */
        return 0;
    }
    ensure_A(n);
    return A[(size_t)n];
}
