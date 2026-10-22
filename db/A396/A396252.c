/*
 * A396252  Conductor of the elliptic curve y^2 = x^3 + 3*n*x + (1 - n^3), the
 *          Kraus-Frey curve for x^3 + y^3 = z^k, at (x,y) = (n,1).
 * https://oeis.org/A396252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^c(n) * 3^2 * r5(n^3+1) where c(n) = 5,3,4,2 for
 *            n==1,2,3,0 (mod 4) and r5(m) = product of distinct primes ≥5
 *            dividing m
 * Range:     u8 none, u16 n <= 8, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PRIME_LIMIT 1000000    /* sqrt(1e12) */
#define MAX_PRIMES 80000       /* number of primes up to PRIME_LIMIT */

/* static storage for primes */
static unsigned char is_composite[PRIME_LIMIT + 1];
static uint32_t primes_arr[MAX_PRIMES];
static size_t primes_count = 0;

/* initialise the prime list once */
static void init_primes(void)
{
    if (primes_count > 0)
        return;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes_arr[primes_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    /* ensure primes are generated */
    init_primes();

    /* determine 2^c(n) */
    static const uint32_t c_arr[4] = {2, 5, 3, 4};   /* n%4 : 0->2,1->5,2->3,3->4 */
    uint32_t c = c_arr[(uint32_t)(n & 3ULL)];
    seq_t pow2c = (seq_t)1;
    for (uint32_t i = 0; i < c; ++i)
        pow2c *= (seq_t)2;

    /* compute m = n^3 + 1 */
    unsigned __int128 m128 = (unsigned __int128)n * n * n + 1;
    uint64_t m64 = (uint64_t)m128;   /* fits for n up to about 1e6 */

    /* compute r5(n^3+1) */
    uint64_t r5 = 1;
    uint64_t temp = m64;
    for (size_t idx = 0; idx < primes_count
         && (uint64_t)primes_arr[idx] * primes_arr[idx] <= temp; ++idx)
    {
        uint32_t p = primes_arr[idx];
        if (p < 5) {
            while (temp % p == 0)
                temp /= p;
            continue;
        }
        if (temp % p == 0) {
            r5 *= p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1 && temp >= 5)
        r5 *= temp;

    /* assemble the result: 2^c * 3^2 * r5 */
    seq_t ans = pow2c;
    ans *= (seq_t)9;          /* 3^2 */
    ans *= (seq_t)r5;
    return ans;
}
