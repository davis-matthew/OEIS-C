/*
 * A378642  Number of numbers m <= n such that phi(m) does not divide n, where
 *          phi is the Euler totient function (A000010).
 * https://oeis.org/A378642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - (number of m ≤ n such that φ(m) divides n)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count numbers m <= n with φ(m) not dividing n.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* n is small enough for a local array (≤ 10000 in tests). */
    size_t N = (size_t)n;

    /* Allocate phi array: phi[i] holds Euler’s totient of i. */
    uint64_t *phi = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!phi)
        return (seq_t)0;          /* allocation failed – return 0 */

    /* Initialize phi[i] = i. */
    for (size_t i = 0; i <= N; ++i)
        phi[i] = i;

    /* Sieve to compute φ(2)…φ(N). */
    for (size_t i = 2; i <= N; ++i) {
        if (phi[i] == i) {            /* i is prime */
            for (size_t j = i; j <= N; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }

    /* Count how many φ(m) divide n. */
    seq_t n_seq = (seq_t)n;
    seq_t count_div = 0;
    for (size_t m = 1; m <= N; ++m) {
        if (n_seq % (seq_t)phi[m] == 0)
            ++count_div;
    }

    free(phi);
    return (seq_t)(n - count_div);
}
