/*
 * A264810  Number of numbers k <= n such that phi(m) = k for some m.
 * https://oeis.org/A264810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2569, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the number of distinct φ(m) values ≤ n
   by sieving φ(m) for m ≤ B where B = 20·n (sufficient for n ≤ 10⁴). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Upper bound for m needed to capture all totient values ≤ n.
       The choice B = 20·n is ample for the required range n ≤ 10 000. */
    idx_t Bidx = n * 20;
    if (Bidx < n + 1) Bidx = n + 1;            /* ensure at least n+1 */
    size_t B = (size_t)Bidx;

    /* Allocate and initialise φ array. */
    seq_t *phi = (seq_t *)malloc((B + 1) * sizeof(seq_t));
    if (!phi) return 0;
    for (size_t i = 0; i <= B; ++i) phi[i] = (seq_t)i;

    /* Euler's totient sieve up to B. */
    for (size_t i = 2; i <= B; ++i) {
        if (phi[i] == (seq_t)i) {              /* i is prime */
            for (size_t j = i; j <= B; j += i) {
                phi[j] -= phi[j] / (seq_t)i;
            }
        }
    }

    /* Boolean array marking which totient values ≤ n have appeared. */
    unsigned char *present = (unsigned char *)calloc((size_t)(n + 1), 1);
    if (!present) { free(phi); return 0; }

    /* Record φ(1)=1 and all φ(i) ≤ n. */
    present[1] = 1;                            /* φ(1) = 1 */
    for (size_t i = 2; i <= B; ++i) {
        seq_t val = phi[i];
        if (val <= (seq_t)n) present[(size_t)val] = 1;
    }

    /* Count distinct totient values ≤ n. */
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (present[(size_t)k]) ++count;
    }

    free(phi);
    free(present);
    return count;
}
