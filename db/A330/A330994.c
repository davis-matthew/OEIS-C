/*
 * A330994  Numerator of P(n)/Q(n) = A000041(n)/A000009(n).
 * https://oeis.org/A330994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330994: numerator of A000041(n)/A000009(n)
   (number of integer partitions divided by number of strict
    integer partitions, in lowest terms). 
   The value is computed for any n ≥ 0.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *P = NULL;          /* partitions p(0..cap‑1)   */
static seq_t *Q = NULL;          /* strict partitions q(0..) */
static size_t cap = 0;           /* current array capacity   */

/* Compute all partition numbers p[0..n] and strict partitions q[0..n]
   using dynamic programming.  Assumes P and Q already point to
   arrays of at least (n+1) elements. */
static void compute_dp(size_t n)
{
    /* initialise to zero */
    for (size_t i = 0; i <= n; ++i) {
        P[i] = 0;
        Q[i] = 0;
    }
    P[0] = 1;
    Q[0] = 1;

    /* ordinary partitions (allow repeated parts) */
    for (size_t part = 1; part <= n; ++part) {
        for (size_t sum = part; sum <= n; ++sum) {
            P[sum] += P[sum - part];
        }
    }

    /* strict partitions (parts distinct) */
    for (size_t part = 1; part <= n; ++part) {
        /* loop downwards to avoid reuse of a part */
        for (size_t sum = n; sum >= part; --sum) {
            Q[sum] += Q[sum - part];
            if (sum == part) break;   /* avoid unsigned underflow */
        }
    }
}

/* Ensure P and Q can store values up to index n.  If a larger size is
   required, reallocates the arrays and recomputes the dp tables. */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap) return;           /* already sufficient */

    /* free old memory if any */
    free(P);
    free(Q);

    P = (seq_t*)calloc(needed, sizeof(seq_t));
    Q = (seq_t*)calloc(needed, sizeof(seq_t));
    if (!P || !Q) exit(EXIT_FAILURE);

    compute_dp(needed - 1);
    cap = needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)(-1)) return 0;   /* defensive for unsigned overflow */
    ensure_capacity(n);
    seq_t p = P[(size_t)n];
    seq_t q = Q[(size_t)n];
    seq_t g = gcd(p, q);
    return p / g;
}
