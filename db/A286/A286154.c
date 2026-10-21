/*
 * A286154  Compound filter: a(n) = T(A055396(n), A000010(n)), where T(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A055396(n)+A000010(n))^2) - A055396(n) -
 *            3*A000010(n))
 * Range:     u8 n <= 12, u16 n <= 306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We sieve up to the current n and keep the prime index (1‑based) and
 * smallest prime factor for each number.  A055396(n) is:
 *   0                       if n == 1
 *   index_of_prime(n)       if n is prime   (prime index of 2 is 1)
 *   1                       if n is composite and even
 *   index_of_prime(spf[n])  if n is composite and odd
 *
 * A000010(n) is Euler's totient φ(n).
 * The above formula turns into a simple integer arithmetic expression
 * using only the operations allowed in the specification.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t max_pre = 0;
static uint32_t *phi      = NULL;         /* φ(n)   */
static uint32_t *omega    = NULL;         /* Ω(n)   (not used but kept from previous precompute) */
static uint8_t  *is_prime = NULL;         /* primality flag */
static idx_t   *spf       = NULL;         /* smallest prime factor */
static idx_t   *prime_idx = NULL;         /* prime index (1‑based) */

static void ensure_precomputed(idx_t N)
{
    if (N <= max_pre) return;

    size_t sz = (size_t)(N + 1);

    /* Resize arrays */
    phi      = (uint32_t*)realloc(phi,      sz * sizeof(uint32_t));
    omega    = (uint32_t*)realloc(omega,    sz * sizeof(uint32_t));
    is_prime = (uint8_t *)realloc(is_prime, sz * sizeof(uint8_t));
    spf      = (idx_t   *)realloc(spf,      sz * sizeof(idx_t));
    prime_idx= (idx_t   *)realloc(prime_idx, sz * sizeof(idx_t));
    if (!phi || !omega || !is_prime || !spf || !prime_idx) return; /* allocation failure */

    /* Initialise */
    for (idx_t i = 0; i <= N; ++i) {
        phi[i]      = (uint32_t)i;
        omega[i]    = 0;
        is_prime[i] = 1;
        spf[i]      = 0;
        prime_idx[i]= 0;
    }
    phi[0] = 0;  phi[1] = 1;
    omega[0] = 0; omega[1] = 0;
    is_prime[0] = 0; is_prime[1] = 0;
    spf[0] = 0; spf[1] = 1;
    prime_idx[0] = 0; prime_idx[1] = 0;

    /* Sieve */
    idx_t *pr = (idx_t*)malloc((N/10 + 1000) * sizeof(idx_t));
    idx_t pc = 0;                     /* number of primes found so far */

    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = i;
            is_prime[i] = 1;
            ++pc;
            pr[pc-1] = i;
            prime_idx[i] = pc;
            phi[i] = (uint32_t)(i-1);
            omega[i] = 1;
        }
        for (idx_t j = 0; j < pc && pr[j] <= spf[i] && i * pr[j] <= N; ++j) {
            idx_t p = pr[j];
            idx_t prod = i * p;
            spf[prod] = p;
            is_prime[prod] = 0;
            /* Euler's totient for composites */
            uint32_t phi_prod;
            if (p == spf[i]) {
                phi_prod = phi[i] * (uint32_t)p;
            } else {
                phi_prod = phi[i] * (uint32_t)(p-1);
            }
            phi[prod] = phi_prod;
            omega[prod] = omega[i] + 1;
        }
    }

    free(pr);
    max_pre = N;
}

seq_t a(idx_t n)
{
    ensure_precomputed(n);

    seq_t phi_n = (seq_t)phi[n];

    /* Compute A055396(n) */
    seq_t x;
    if (n == 1) {
        x = (seq_t)0;
    } else if (is_prime[n]) {
        x = (seq_t)prime_idx[n];
    } else if ((n & 1) == 0) {           /* even composite */
        x = (seq_t)1;
    } else {                             /* odd composite */
        /* spf[n] is the smallest prime factor */
        x = (seq_t)prime_idx[spf[n]];
    }

    /* Apply the formula */
    seq_t sum = x + phi_n;
    seq_t num = sum * sum + (seq_t)2 - x - (seq_t)3 * phi_n;
    return num / (seq_t)2;
}
