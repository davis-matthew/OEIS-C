/*
 * A355442  a(n) = gcd(A003961(n), A276086(n)), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p), and A276086 is primorial
 *          base exp-function.
 * https://oeis.org/A355442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003961(n), A276086(n))
 * Range:     u8 n <= 99, u16 n <= 6236, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

#define MAX_K 20        /* maximum number of primorial digits needed for n ≤ 10⁴ */

/* static state for sieve and prime table */
static bool initialized = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* initialise the list of primes once */
static void init_resources(void)
{
    if (initialized)
        return;

    /* a modest limit that covers primes up to 200 000 and all primes needed
       for n up to at least 10⁴ */
    const idx_t PRIME_LIMIT = 200000;
    build_primes(PRIME_LIMIT, &primes, &prime_cnt);

    initialized = true;
}

seq_t a(idx_t n)
{
    init_resources();

    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1 */

    /* --------------------------------------------------------------------
       Find K and the primorials prim[0..K]  (prim[0] = 1)
       -------------------------------------------------------------------- */
    seq_t prim[MAX_K + 1];
    int K = 0;
    prim[0] = (seq_t)1;
    while (K < MAX_K) {
        seq_t next = prim[K] * (seq_t)primes[K];
        if (next > n)          /* stop before exceeding n */
            break;
        ++K;
        prim[K] = next;
    }

    /* --------------------------------------------------------------------
       Exponents of the first K primes in n  (prime[0]..prime[K-1])
       -------------------------------------------------------------------- */
    int exp_val[MAX_K] = {0};
    uint64_t temp_factor = n;
    for (int i = 0; i < K; ++i) {
        uint64_t p = primes[i];
        int cnt = 0;
        while (temp_factor % p == 0) {
            temp_factor /= p;
            ++cnt;
        }
        exp_val[i] = cnt;
    }

    /* --------------------------------------------------------------------
       Digits of n in primorial base  (digits[0..K])
       -------------------------------------------------------------------- */
    seq_t digits[MAX_K + 1];
    seq_t rem = (seq_t)n;
    for (int i = K; i >= 0; --i) {
        seq_t d = rem / prim[i];
        digits[i] = d;
        rem -= d * prim[i];
    }

    /* --------------------------------------------------------------------
       Compute the gcd as product of primes p_k raised to
       min(exp of p_{k-1} in n , digits[k])  for k = 1 .. K
       -------------------------------------------------------------------- */
    seq_t result = (seq_t)1;
    for (int k = 1; k <= K; ++k) {
        int e = exp_val[k - 1];
        int d = (int)digits[k];
        int m = (e < d) ? e : d;
        if (m > 0) {
            seq_t pw = seq_pow((seq_t)primes[k], (idx_t)m);
            result = seq_mul(result, pw);
        }
    }

    return result;
}
