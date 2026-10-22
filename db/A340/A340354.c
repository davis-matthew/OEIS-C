/*
 * A340354  a(n) is the sum, for j < n coprime to n, of the least prime > n
 *          congruent to j (mod n).
 * https://oeis.org/A340354
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum_{1 <= j < n, gcd(j,n)=1} p_j where p_j is the least
 *            prime > n congruent to j (mod n)
 * Range:     u8 n <= 10, u16 n <= 108, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   gcd_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_val.h"

#define PRIMELIMIT 500000

static uint8_t sieve[PRIMELIMIT + 1];
static idx_t primes[PRIMELIMIT + 1];
static size_t primesCnt = 0;
static bool primesInit = false;

/* Compute Euler's totient of m (m <= 10000 here) */
static idx_t phi_val(idx_t m)
{
    idx_t result = m;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            result = result / p * (p - 1);
            do {
                temp /= p;
            } while (temp % p == 0);
        }
    }
    if (temp > 1)
        result = result / temp * (temp - 1);
    return result;
}

/* Sieve of Eratosthenes to generate all primes <= PRIMELIMIT once */
static void init_primes(void)
{
    if (primesInit)
        return;
    for (size_t i = 0; i <= PRIMELIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIMELIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIMELIMIT; j += i)
                sieve[j] = 0;
    for (size_t i = 2; i <= PRIMELIMIT; ++i)
        if (sieve[i])
            primes[primesCnt++] = (idx_t)i;
    primesInit = true;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2, return 0 for safety */

    init_primes();

    idx_t needed = phi_val(n);
    idx_t hit = 0;
    seq_t sum = 0;

    /* allocate visited array for residues */
    uint8_t *seen = (uint8_t *)calloc((size_t)n, 1);
    if (!seen)
        return 0;   /* allocation failure */

    /* find first prime > n */
    size_t pi = 0;
    while (pi < primesCnt && primes[pi] <= n)
        ++pi;

    for (; pi < primesCnt && hit < needed; ++pi) {
        idx_t p = primes[pi];
        idx_t r = p % n;
        if (r == 0)
            continue;
        if (seen[r])
            continue;
        if (gcd_val(r, n) != 1)
            continue;
        seen[r] = 1;
        ++hit;
        sum += (seq_t)p;
    }

    free(seen);
    return sum;
}
