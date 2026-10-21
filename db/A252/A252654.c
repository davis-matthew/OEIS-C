/*
 * A252654  Number of multisets of nonempty words with a total of n letters
 *          over n-ary alphabet.
 * https://oeis.org/A252654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{j≥1} 1/(1-x^j)^(n^j) Recurrence (Euler
 *            transform): a(0)=1, a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d·n^d )
 *            · a(n-k). Requires integer arithmetic only
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        seq_t one = (seq_t)1;
        return one;
    }

    /* Allocate arrays a[0..n] and B[0..n] */
    size_t len = (size_t)(n + 1);
    seq_t *A = alloc_seq_array(len);
    seq_t *B = alloc_seq_array(len);
    if (!A || !B) { /* fallback: 0 on allocation failure */
        return (seq_t)0;
    }

    /* Initialise */
    A[0] = (seq_t)1;        /* a(0) = 1 */
    B[0] = (seq_t)0;        /* unused */

    /* Compute B[k] = Σ_{d|k} d·n^d using a sieve‑style approach */
    seq_t pow_n = (seq_t)n; /* n^1 */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t term = (seq_t)d * pow_n;          /* d·n^d */
        for (idx_t m = d; m <= n; m += d) {
            B[m] += term;
        }
        pow_n *= (seq_t)n;                      /* advance to n^(d+1) */
    }

    /* Compute a[1..n] using the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += B[k] * A[i - k];
        }
        A[i] = sum / (seq_t)i;                  /* division is exact */
    }

    seq_t result = A[n];

    /* Free allocated memory */
    free_seq_array(A);
    free_seq_array(B);

    return result;
}
