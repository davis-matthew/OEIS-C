/*
 * A286254  Compound filter: a(n) = P(A001511(n), A055396(1+n)), where P(n,k)
 *          is sequence A000027 used as a pairing function.
 * https://oeis.org/A286254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*( 2 + (A001511(n)+A055396(1+n))^2 - A001511(n) -
 *            3*A055396(1+n) )
 * Range:     u8 n <= 39, u16 n <= 2415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t *spf = NULL;             /* smallest prime factor of each integer */
static idx_t *prime_idx_arr = NULL;   /* index of the least prime factor of each integer */
static idx_t precomp_limit = 0;       /* maximum value of n+1 already pre‑computed */

/* Pre‑compute primes, smallest prime factors and prime indices up to @limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= precomp_limit) return;

    /* Resize storage */
    idx_t new_size = limit + 1;          /* indices 0..limit inclusive */
    spf          = (idx_t *)realloc(spf,            new_size * sizeof(idx_t));
    prime_idx_arr= (idx_t *)realloc(prime_idx_arr,   new_size * sizeof(idx_t));

    /* Initialize */
    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = 0;
        prime_idx_arr[i] = 0;
    }

    /* Sieve of Eratosthenes with smallest prime factor */
    idx_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            ++prime_count;
            prime_idx_arr[i] = prime_count;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = (idx_t)i;
                }
            }
        }
    }

    /* For composites, set prime index to the index of their smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (prime_idx_arr[i] == 0) {
            prime_idx_arr[i] = prime_idx_arr[spf[i]];
        }
    }

    precomp_limit = limit;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; guard against n == 0 just in case */
    if (n == 0) return 0;

    /* Ensure prime data is available up to n+1 */
    ensure_primes(n + 1);

    /* A001511(n) = v2(n) + 1, the exponent of 2 dividing n plus one */
    seq_t x = (seq_t)((unsigned long long)__builtin_ctzll((unsigned long long)n) + 1);

    /* A055396(n+1) = index of the least prime factor of n+1 */
    seq_t y = (seq_t)prime_idx_arr[n + 1];

    /* Compute the closed‑form expression */
    seq_t sum = x + y;
    seq_t sq  = sum * sum;                      /* (x + y)^2                                 */
    seq_t val = (sq - x - ((seq_t)3 * y) + 2) / 2;
    return val;
}
