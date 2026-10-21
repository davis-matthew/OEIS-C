/*
 * A276376  Exponent of highest power of 3 dividing product of composite
 *          numbers between n-th prime and (n+1)-st prime.
 * https://oeis.org/A276376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 3 in product of composite numbers in (p_n ,
 *            p_{n+1})
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* precompute up to a limit sufficient for n <= 10000 */
static const idx_t LIMIT = 200000;      /* covers the 104729th prime comfortably */
static seq_t *v3 = NULL;                /* exponent of 3 for each number */
static bool *is_prime = NULL;           /* primality flag */
static uint64_t *primes = NULL;         /* list of primes */
static size_t prime_cnt = 0;            /* number of primes */
static bool init_done = false;

static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* allocate flags and v3 array */
    is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    v3 = (seq_t*)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!is_prime || !v3) {
        exit(EXIT_FAILURE);
    }

    /* simple sieve of Eratosthenes */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* build prime list */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;
    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        exit(EXIT_FAILURE);
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    /* precompute exponent of 3 for each number */
    v3[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        seq_t e = 0;
        idx_t t = i;
        while (t % 3 == 0) {
            ++e;
            t /= 3;
        }
        v3[i] = e;
    }
}

seq_t a(idx_t n)
{
    init();
    if (n < 1 || n >= prime_cnt) return 0;
    uint64_t p_n     = primes[n - 1];
    uint64_t p_next  = primes[n];
    idx_t start = p_n + 1;
    idx_t end   = p_next;          /* exclusive */

    seq_t sum = 0;
    for (idx_t i = start; i < end; ++i) {
        if (!is_prime[i]) sum += v3[i];
    }
    return sum;
}
