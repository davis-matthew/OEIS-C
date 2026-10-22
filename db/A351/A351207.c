/*
 * A351207  Number of ordered pairs of divisors of n whose sum is squarefree.
 * https://oeis.org/A351207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|n, d2|n} mu(d1 + d2)^2 where mu(x)^2 is 1 if x is
 *            square‑free, 0 otherwise. The function enumerates all divisor
 *            pairs of n and counts those whose sum is square‑free
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum possible value of d1 + d2 for n <= 10000 is 20000. */
#define MAX_SUM 20000

static bool squarefree[MAX_SUM + 1];
static bool initialized = false;

/* Initialise array that marks whether an integer up to MAX_SUM
   is square‑free. */
static void init_squarefree(void)
{
    /* Sieve of Eratosthenes for primes up to MAX_SUM */
    bool is_prime[MAX_SUM + 1];
    for (int i = 0; i <= MAX_SUM; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= MAX_SUM; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_SUM; j += i)
                is_prime[j] = false;

    /* All numbers are initially considered square‑free */
    for (int i = 0; i <= MAX_SUM; ++i)
        squarefree[i] = true;

    /* Any multiple of p^2 (p prime) is not square‑free */
    for (int p = 2; p <= MAX_SUM; ++p)
        if (is_prime[p]) {
            int sq = p * p;
            for (int j = sq; j <= MAX_SUM; j += sq)
                squarefree[j] = false;
        }
}

/* Compute the a(n) value for a given n >= 1. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_squarefree();
        initialized = true;
    }

    idx_t nn = n;                /* local copy of n */
    int divisors[200];           /* enough for all n <= 10000 */
    int dcnt = 0;

    /* Find all divisors of nn */
    for (idx_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            divisors[dcnt++] = (int)i;
            if (i != nn / i)
                divisors[dcnt++] = (int)(nn / i);
        }
    }

    seq_t count = 0;

    /* Count ordered pairs with square‑free sum */
    for (int a = 0; a < dcnt; ++a) {
        for (int b = 0; b < dcnt; ++b) {
            int sum = divisors[a] + divisors[b];
            if (squarefree[sum])
                count += 1;
        }
    }

    return count;
}
