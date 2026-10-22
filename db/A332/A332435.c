/*
 * A332435  Row sums of the irregular triangle A332434. a(n) equals the number
 *          of odd numbers <= n, of the smallest nonnegative reduced residue
 *          system modulo (2*n + 1), for n >= 1.
 * https://oeis.org/A332435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odd integers r with 1 ≤ r ≤ n and gcd(r,2n+1)=1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by inclusion–exclusion over the prime factors of 2n+1.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MAX_PRIME_LIMIT
#define MAX_PRIME_LIMIT 20001
#endif

static uint32_t primes_tbl[MAX_PRIME_LIMIT / 2];
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool sieve[MAX_PRIME_LIMIT];
    for (size_t i = 0; i < MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i < MAX_PRIME_LIMIT; ++i)
        if (sieve[i]) {
            for (size_t j = i * i; j < MAX_PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    for (size_t i = 2; i < MAX_PRIME_LIMIT; ++i)
        if (sieve[i]) primes_tbl[primes_cnt++] = (uint32_t)i;
    primes_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_primes();

    /* b = 2n + 1 is odd */
    idx_t b = (idx_t)2 * n + 1;
    idx_t temp = b;
    idx_t facts[16];
    size_t fcnt = 0;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes_tbl[i] * primes_tbl[i] <= temp; ++i) {
        uint64_t p = primes_tbl[i];
        if (temp % p == 0) {
            facts[fcnt++] = (idx_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) facts[fcnt++] = temp;

    /* count of odd numbers ≤ n */
    idx_t odd_total = (n + 1) / 2;
    seq_t res = (seq_t)odd_total;

    /* Inclusion–exclusion over subsets of prime factors */
    size_t subsets = (size_t)1 << fcnt;
    for (size_t mask = 1; mask < subsets; ++mask) {
        idx_t prod = 1;
        size_t bits = 0;
        for (size_t j = 0; j < fcnt; ++j) {
            if (mask & ((size_t)1 << j)) {
                prod *= facts[j];
                ++bits;
            }
        }
        idx_t cnt = ((n / prod) + 1) / 2;   /* odd multiples of prod <= n */
        if (bits & 1) {
            res -= (seq_t)cnt;
        } else {
            res += (seq_t)cnt;
        }
    }
    return res;
}
