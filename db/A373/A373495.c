/*
 * A373495  a(1) = 2; thereafter, a(n) = prime(n)^prime(n-1) (mod 10).
 * https://oeis.org/A373495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^prime(n-1) (mod 10), a(1)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by sieving the first n primes and then modular exponentiation. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined below offset */
    if (n == 1) return (seq_t)2;      /* special value */

    /* Ensure we have at least n primes. */
    if ((size_t)n > primes_cnt) {
        uint32_t *new_arr = NULL;
        size_t new_cnt = 0;
        sieve_first_n_primes((idx_t)n, &new_arr, &new_cnt);
        free(primes);                  /* free previous allocation */
        primes = new_arr;
        primes_cnt = new_cnt;
    }

    /* prime(n) is primes[n-1], prime(n-1) is primes[n-2] */
    uint32_t p_n   = primes[n-1];
    uint32_t p_nm1 = primes[n-2];

    /* Modular exponentiation mod 10 */
    unsigned int base = p_n % 10;
    unsigned int exp  = p_nm1;
    unsigned int res  = 1;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % 10;
        base = (base * base) % 10;
        exp >>= 1;
    }

    return (seq_t)res;
}
