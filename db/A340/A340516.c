/*
 * A340516  Let p_i (i=1..m) denote the primes <= n, and let e_i be the
 *          maximum number such that p_i^e_i <= n; then a(n) =
 *          Product_{i=1..m} p_i^(2*e_i-1).
 * https://oeis.org/A340516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n} p^(2·e_p−1), where e_p = max{e : p^e ≤ n}
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A340516. */

#include <stdint.h>
#include <stdlib.h>

static seq_t pow_seq(seq_t base, unsigned long long exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset handling */

    /* Allocate sieve array: 0 = composite, 1 = possibly prime */
    size_t N = (size_t)n;
    char *is_prime = (char*)malloc((N + 1) * sizeof(char));
    if (!is_prime) return 0;           /* allocation failure */
    for (size_t i = 0; i <= N; ++i) is_prime[i] = 1;
    if (N >= 0) is_prime[0] = 0;
    if (N >= 1) is_prime[1] = 0;

    /* Simple Eratosthenes sieve */
    for (size_t p = 2; p * p <= N; ++p) {
        if (!is_prime[p]) continue;
        for (size_t multiple = p * p; multiple <= N; multiple += p)
            is_prime[multiple] = 0;
    }

    seq_t prod = (seq_t)1;

    for (size_t p = 2; p <= N; ++p) {
        if (!is_prime[p]) continue;
        uint64_t prime = (uint64_t)p;

        /* Determine e_p: largest exponent such that prime^e ≤ n */
        unsigned long long e = 0;
        unsigned long long power = 1;
        while (power <= n / prime) {
            power *= prime;
            ++e;
        }

        unsigned long long term_exp = 2 * e - 1;   /* 2*e_p−1 */
        seq_t term = pow_seq((seq_t)prime, term_exp);
        prod = prod * term;
    }

    free(is_prime);
    return prod;
}
