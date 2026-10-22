/*
 * A330030  Least k such that Sum_{i=0..n} k^n / i! is a positive integer.
 * https://oeis.org/A330030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(A061355(n)) – least k>0 with k^n / A061355(n)
 *            integer where A061355(n) is the denominator of Sum_{i=0..n} 1/i!
 *            in lowest terms and A007947 gives the product of distinct prime
 *            factors
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 28, u64 n <= 53, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special cases: a(0) = a(1) = 1 */
    if (n < 2) return (seq_t)1;

    /* ---------- Sieve of Eratosthenes up to n ---------- */
    idx_t limit = n;
    char *is_prime = (char *)malloc((limit + 1) * sizeof(char));
    if (!is_prime) return (seq_t)0;          /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes and store them */
    idx_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;
    idx_t *primes = (idx_t *)malloc(primes_cnt * sizeof(idx_t));
    idx_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pc++] = i;
    free(is_prime);

    /* ---------- Compute a(n) ---------- */
    seq_t ans = (seq_t)1;

    for (idx_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];

        /* Compute e = v_p(n!) */
        idx_t e = 0;
        if (p <= n) {
            idx_t pw = p;
            while (pw <= n) {
                e += n / pw;
                if (pw > n / p) break;
                pw *= p;
            }
        }

        /* Compute modulus mp = p^e */
        seq_t mp = (seq_t)1;
        for (idx_t i = 0; i < e; ++i)
            mp *= (seq_t)p;

        /* Compute r = B(n) mod mp where B(n) = n! * Sum_{i=0..n} 1/i! */
        seq_t r = (seq_t)1; /* B(0) = 1 */
        for (idx_t k = 1; k <= n; ++k) {
            r = ((seq_t)k * r + 1) % mp;
        }

        /* If r != 0, prime p appears in rad(D(n)) */
        if (r != (seq_t)0)
            ans = ans * (seq_t)p;
    }

    free(primes);
    return ans;
}
