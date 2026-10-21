/*
 * A294098  Number of partitions of 2n into two parts such that one part is
 *          squarefree and the other part is nonsquarefree.
 * https://oeis.org/A294098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - Sum_{i=1..n} [c(i) = c(2*n-i)]
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* a(idx_t n) returns the n-th term of OEIS sequence A294098
 *  (Number of partitions of 2n into two parts such that one part is
 *   squarefree and the other part is nonsquarefree).  The function
 *   works for any idx_t (uint64_t) and seq_t (an integer type or
 *   arbitrary‑precision class).  It is O(n) per call with a small
 *   sieve of size 2n. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, harmless */

    /* Work with the integer 2*n (fits in idx_t). */
    const idx_t m = n * 2;

    /* Allocate an array that marks squarefreeness of numbers 0..m.
       sqfree[i] = 1  iff i is squarefree, 0 otherwise. */
    uint8_t *sqfree = (uint8_t *)malloc((size_t)(m + 1));
    if (!sqfree) exit(1);
    for (idx_t i = 1; i <= m; ++i) sqfree[i] = 1;
    sqfree[0] = 0;

    /* Simple sieve to identify squares of primes.  We also create a
       bool array of primes up to m to reuse the loop. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(m + 1));
    if (!is_prime) { free(sqfree); exit(1); }
    for (idx_t i = 2; i <= m; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* For every prime p <= sqrt(m), mark multiples of p*p as
       having a squared prime factor (hence not squarefree). */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t k = p * 2; k <= m; k += p) is_prime[k] = 0;
        for (idx_t k = p * p; k <= m; k += p * p) sqfree[k] = 0;
    }

    /* Count the number of i with squarefreeness equal to that of
       2n-i. */
    idx_t equal_cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (sqfree[i] == sqfree[2 * n - i]) ++equal_cnt;
    }

    /* The requested term. */
    const idx_t result = n - equal_cnt;

    free(is_prime);
    free(sqfree);
    return (seq_t)result;
}
