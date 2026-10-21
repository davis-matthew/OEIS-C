/*
 * A280391  Number of 2 X 2 matrices with all elements in {0,...,n} with
 *          permanent = determinant * n.
 * https://oeis.org/A280391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 124, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A280391:
 * Number of 2 × 2 matrices with entries in {0,…,n} whose permanent equals
 * n times the determinant
 *
 * The following code counts the solutions of  
 *          (n‑1)·x·y = (n+1)·z·w     (0 ≤ x,y,z,w ≤ n).
 * Let g = gcd(n‑1,n+1) (1 if n even, 2 if n odd) and set
 *     l = (n‑1)/g,   k = (n+1)/g   (gcd(l,k) = 1).
 * Then l·x·y = k·z·w → for each s ≥ 0,
 *         x·y = k·s  and  z·w = l·s.
 *   a(n) = Σ_{s=0}^{⌊n²/k⌋}  P(k·s) · P(l·s)
 * where P(v) = number of ordered pairs (u,v) in [0,n]² with u·v = v.
 *
 * The algorithm pre‑computes the frequencies of products
 * 0…n² that are multiples of k (and, if l≠0, multiples of l)
 * by iterating over all (a,b) pairs once.
 * Complexity: O((n+1)²) operations, ~1e8 for n=10⁴.
 * Memory: O(n) for the frequency arrays.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 a single zero‑matrix satisfies the condition */
    if (n == 0) return (seq_t)1;

    /* Common parameters */
    idx_t g = (n & 1) ? 2 : 1;     /* gcd(n‑1,n+1) */
    idx_t l = (n - 1) / g;
    idx_t k = (n + 1) / g;

    /* Upper bound for s: k·s ≤ n² */
    idx_t maxS = (n * n) / k;      /* floor(n² / k) */
    size_t size = (size_t)(maxS + 1);

    /* Allocate frequency arrays */
    seq_t *freqk = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *freql = (seq_t *)calloc(size, sizeof(seq_t));

    if (!freqk || !freql) {
        /* allocation failed – return 0 (undefined behaviour acceptable here) */
        return (seq_t)0;
    }

    /* Special case: l = 0 occurs only for n = 1.
       Then z·w must be zero for all s, and its count is constant. */
    if (l == 0) {
        seq_t zeroCount = (seq_t)(2ULL * n + 1);  /* number of pairs (z,w) with product 0 */
        for (size_t i = 0; i < size; ++i) freql[i] = zeroCount;
    }

    /* Main double loop over all (a,b) pairs to populate freqk and freql */
    for (idx_t a = 0; a <= n; ++a) {
        for (idx_t b = 0; b <= n; ++b) {
            idx_t prod = a * b;    /* prod ≤ n² ≤ 1e8 for n ≤ 1e4 */

            /* Count pairs for k */
            if (prod % k == 0) {
                idx_t s = prod / k;
                if (s <= maxS) ++freqk[s];
            }

            /* Count pairs for l (only if l ≠ 0) */
            if (l != 0 && prod % l == 0) {
                idx_t s = prod / l;
                if (s <= maxS) ++freql[s];
            }
        }
    }

    /* Compute the final sum */
    seq_t result = 0;
    for (size_t s = 0; s < size; ++s) {
        result += freqk[s] * freql[s];
    }

    /* Clean up */
    free(freqk);
    free(freql);

    return result;
}
