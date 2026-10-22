/*
 * A389595  a(n) is the permanent of the Vandermonde matrix of the distinct
 *          prime factors of n.
 * https://oeis.org/A389595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of the Vandermonde matrix of the distinct prime
 *            factors of n. That is, if n = p1 * p2 * … * pk with pi distinct
 *            primes, then a(n) = ∑_{σ∈S_k} ∏_{i=1}^k pi^{σ(i)-1}
 * Range:     u8 n <= 41, u16 n <= 209, u32 n <= 2729, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Generate the next lexicographic permutation of array a of length n.
   Returns 1 if a new permutation was produced, 0 if a was the last
   permutation (already in descending order). */
static int next_permutation(size_t *a, size_t n)
{
    if (n < 2) return 0;
    size_t i = n - 1;
    while (i > 0 && a[i - 1] >= a[i]) --i;
    if (i == 0) return 0;
    size_t j = n - 1;
    while (a[j] <= a[i - 1]) --j;
    size_t t = a[i - 1];
    a[i - 1] = a[j];
    a[j] = t;
    for (size_t L = i, R = n - 1; L < R; ++L, --R) {
        t = a[L];
        a[L] = a[R];
        a[R] = t;
    }
    return 1;
}

/* Store the distinct prime factors of n in primes[16].
   *cnt will be set to the number of distinct primes found.
   The array size 16 is sufficient for n ≤ 10 000. */
static void distinct_prime_factors(uint64_t n, uint64_t primes[16], size_t *cnt)
{
    size_t k = 0;
    if (n <= 1) { *cnt = 0; return; }

    if (n % 2 == 0) {
        primes[k++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[k++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[k++] = n;
    *cnt = k;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;

    uint64_t primes[16];
    size_t k;
    distinct_prime_factors((uint64_t)n, primes, &k);

    if (k <= 1) return (seq_t)1;

    /* Pre‑compute powers p_i^e for e = 0 .. k-1 */
    seq_t powers[16][16];
    for (size_t i = 0; i < k; ++i) {
        powers[i][0] = 1;
        for (size_t e = 1; e < k; ++e)
            powers[i][e] = powers[i][e - 1] * (seq_t)primes[i];
    }

    /* Enumerate all permutations of the exponent set {0 .. k-1} */
    size_t perm[16];
    for (size_t i = 0; i < k; ++i) perm[i] = i;

    seq_t sum = 0;
    do {
        seq_t prod = 1;
        for (size_t i = 0; i < k; ++i)
            prod *= powers[i][perm[i]];
        sum += prod;
    } while (next_permutation(perm, k));

    return sum;
}
