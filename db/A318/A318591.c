/*
 * A318591  Number of n-member subsets of [3*n] whose elements sum to a
 *          multiple of three.
 * https://oeis.org/A318591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318591
 *  a(n) = number of n‑member subsets of [3*n] whose elements sum to a multiple of 3.
 *
 *  Computation:
 *  Let n be the size of the subset and 3n the range.
 *  There are n numbers ≡0 (mod 3), n numbers ≡1, and n numbers ≡2.
 *  A subset with a elements from the “0” class,
 *  b elements from the “1” class,
 *  c elements from the “2” class satisfies a+b+c = n
 *  and has sum ≡ b+2c (mod 3).
 *  The sum is 0 iff b ≡ c (mod 3).
 *  Thus a(n) equals the sum of C(n,a)·C(n,b)·C(n,c) over all
 *  non‑negative a,b,c with a+b+c = n and b ≡ c (mod 3).
 *
 *  This implementation precomputes all binomial coefficients C(n,k)
 *  (using the multiplicative formula) in O(n) time and then evaluates
 *  the double sum in O(n²) time with exactly the required integer
 *  operations.  The algorithm is valid for all integer types seq_t
 *  and idx_t supported by the compiler and satisfies the stated
 *  performance constraints.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array of binomial coefficients C(n,k), 0 ≤ k ≤ n */
    size_t len = (size_t)(n + 1);
    seq_t *C = (seq_t*)calloc(len, sizeof(seq_t));
    if (!C) return 0;                         /* allocation failure */

    C[0] = 1;
    for (idx_t k = 1; k <= n; ++k)
        C[k] = C[k - 1] * ((seq_t)(n - k + 1)) / (seq_t)k;

    seq_t ans = 0;

    for (idx_t b = 0; b <= n; ++b) {
        idx_t r = b % 3;                      /* residue of c must equal r */
        idx_t max_c = n - b;                  /* a = n - b - c ≥ 0 */

        for (idx_t c = r; c <= max_c; c += 3) {
            idx_t a = n - b - c;              /* a = n - b - c ≥ 0 */
            seq_t prod = C[a] * C[b] * C[c];  /* multiplication in seq_t */
            ans += prod;                     /* add to the final answer */
        }
    }

    free(C);
    return ans;
}
