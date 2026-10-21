/*
 * A295659  Number of exponents larger than 2 in the prime factorization of n.
 * https://oeis.org/A295659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime exponents > 2 in the prime factorisation
 *            of n (OEIS A295659)
 * Range:     u8 n <= 9331, u16 n <= 9331, u32 n <= 9331, u64 n <= 9331,
 *            u128 n <= 9331, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Sieve limits – primes up to 100 000 are more than enough for n up to about
   10^10 and for the time limits requested. */
enum { PRIME_LIMIT = 100000 };
static uint32_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* Initialise the prime table once.  */
static void init_primes(void)
{
    if (primes_inited)
        return;
    static uint8_t comp[PRIME_LIMIT + 1];
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    primes_inited = true;
}

/* Count exponents larger than 2 in the prime factorisation of n. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    init_primes();

    seq_t cnt = 0;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp > 2)
                ++cnt;
        }
    }
    /* Remaining factor (if >1) is either a prime of exponent 1 or 1. */
    return cnt;
}
