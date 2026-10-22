/*
 * A354035  a(n) = 1 if n is odd and sigma(n^2) == 3 (mod 4), otherwise 0.
 * https://oeis.org/A354035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8337, u16 n <= 8337, u32 n <= 8337, u64 n <= 8337,
 *            u128 n <= 8337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A354035
   a(n) = 1  if n is odd and σ(n²) ≡ 3 (mod 4),
          0  otherwise.
   (σ is the sum‑of‑divisors function) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Prime sieve ------------------------------------------------- */
/* ------------------------------------------------------------ */

#define MAX_PRIME 1000000  /* sufficient for n ≤ 10 000 */

static uint32_t *primes = NULL;   /* array of primes up to MAX_PRIME  */
static size_t  prime_count = 0;

/* One‑time initialization of the prime table. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Allocate array for sieve. */
    char *is_comp = (char *)malloc(MAX_PRIME + 1);
    if (!is_comp) abort();  /* allocation failure */
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_comp[i] = 0;

    /* Compute integer sqrt of MAX_PRIME without floating point. */
    size_t lim = 0;
    while ((lim + 1) * (lim + 1) <= MAX_PRIME) ++lim;

    /* Mark composites. */
    for (size_t i = 2; i <= lim; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    /* Allocate and store primes. */
    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) abort();
    prime_count = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            primes[prime_count++] = (uint32_t)i;
        }
    }

    free(is_comp);
}

/* ------------------------------------------------------------ */
/* Compute σ(n²) modulo 4 ------------------------------------- */
/* ------------------------------------------------------------ */

static unsigned char sigma_n2_mod4(idx_t n)
{
    init_primes();

    unsigned char result = 1;          /* product modulo 4 */
    uint64_t m = n;                    /* intermediate work */

    for (size_t pi = 0; pi < prime_count &&
                       (uint64_t)primes[pi] * primes[pi] <= m; ++pi) {
        uint32_t p = primes[pi];
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; ++e; }

            /* Compute Σ_{k=0}^{2e} p^k (mod 4). */
            unsigned char sum_mod = 1;          /* power 0 */
            unsigned char term    = 1;          /* current p^k (mod 4) */
            unsigned char p_mod   = p & 3;      /* p % 4 */

            for (unsigned k = 1; k <= 2 * e; ++k) {
                term = (unsigned char)((term * p_mod) & 3);
                sum_mod += term;
                sum_mod &= 3;
            }
            result = (unsigned char)((result * sum_mod) & 3);
        }
    }

    if (m > 1) {   /* remaining prime factor with exponent 1 */
        unsigned char p_mod = (unsigned char)(m & 3);
        unsigned char sum_mod = 1;
        unsigned char term = 1;
        for (unsigned k = 1; k <= 2; ++k) {
            term = (unsigned char)((term * p_mod) & 3);
            sum_mod += term;
            sum_mod &= 3;
        }
        result = (unsigned char)((result * sum_mod) & 3);
    }

    return result;
}

/* ------------------------------------------------------------ */
/* Public sequence function ----------------------------------- */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* sequence offset = 1 */

    if ((n & 1ULL) == 0) return (seq_t)0;   /* even numbers give 0 */

    unsigned char sigma_mod4 = sigma_n2_mod4(n);
    return (sigma_mod4 == 3) ? (seq_t)1 : (seq_t)0;
}
