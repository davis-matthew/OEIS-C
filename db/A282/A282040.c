/*
 * A282040  Let p = n-th prime == 7 mod 8; a(n) = sum of quadratic residues
 *          mod p that are > p/2.
 * https://oeis.org/A282040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of quadratic residues modulo the n-th prime ≡ 7 (mod
 *            8) that are greater than p/2 (where p is that prime)
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 3323, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* memoised list of primes ≡ 7 (mod 8) */
static uint64_t *prime7 = NULL;
static size_t   prime7_count = 0;
static size_t   prime7_cap   = 0;

/* ensure we have at least `need` primes ≡ 7 (mod 8) in prime7 */
static void ensure_prime7(idx_t need)
{
    /* initialise with the first such prime (7) */
    if (prime7_count == 0)
    {
        if (prime7_cap == 0) {
            prime7_cap = 128;
            prime7 = (uint64_t*)malloc(prime7_cap * sizeof(uint64_t));
            if (!prime7) exit(1);
        }
        prime7[prime7_count++] = 7;
    }

    while (prime7_count < (size_t)need)
    {
        uint64_t last = prime7[prime7_count - 1];
        uint64_t p = next_prime(last);  /* next prime after last */
        while (p % 8 != 7)              /* skip until 7 mod 8 */
            p = next_prime(p);

        if (prime7_count == prime7_cap) {
            prime7_cap *= 2;
            uint64_t *tmp = (uint64_t*)realloc(prime7, prime7_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            prime7 = tmp;
        }
        prime7[prime7_count++] = p;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset not used for n < 1 */

    /* obtain the required prime p */
    ensure_prime7(n);
    uint64_t p64 = prime7[n - 1];
    seq_t   p   = (seq_t)p64;
    seq_t   half = p / 2;

    seq_t sum = 0;
    idx_t limit = p64 / 2;           /* (p-1)/2, since p is odd */
    for (idx_t x = 1; x <= limit; ++x)
    {
        seq_t r = ((seq_t)x * (seq_t)x) % p;
        if (r > half) sum += r;
    }
    return sum;
}
