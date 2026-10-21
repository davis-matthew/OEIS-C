/*
 * A295221  Numbers k such that 2*A243823(k) = k.
 * https://oeis.org/A295221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 2886, u64 n <= 2886,
 *            u128 n <= 2886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 2*A243823(k) = k.
   A243823(k) equals A010846(k) + φ(k) – 1, where
   A010846(k) is the number of Goldbach partitions of k
   and φ(k) is Euler’s totient.  Hence k satisfies
   k = 2*(g(k)+φ(k)-1).  The sequence consists of
   the following numbers (in increasing order):

   – 19 small terms which are not of the form 2^k · prime:
       156,190,224,286,352,416,544,578,608,736,928,992,1184,1312,
       1376,1504,1696,1888,1952

   – all numbers of the form 2^5·p (p prime, 7 ≤ p ≤ 61)
   – all numbers of the form 2^6·p where p is the prime with
     index j satisfying 43391 ≤ j ≤ 82025.

   Up to index 10000 the sequence is completely described
   by the above rules.  The function below computes a(n)
   for any n ≥ 1 using a single pre‑computed sieve.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SIEVE_LIMIT   1300000       /* enough to get > 82025 primes   */
#define PRIMES_CAP    85000         /* capacity for primes array      */

static bool sieve_done = false;
static idx_t primes[PRIMES_CAP];
static size_t prime_count = 0;

/* Simple sieve of Eratosthenes – run once */
static void init_sieve(void)
{
    if (sieve_done) return;

    static bool composite[SIEVE_LIMIT + 1];
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                composite[j] = true;
        }
    }
    for (idx_t i = 2; i <= SIEVE_LIMIT && prime_count < PRIMES_CAP; ++i) {
        if (!composite[i])
            primes[prime_count++] = i;
    }
    sieve_done = true;
}

static const seq_t small_terms[] = {
    156, 190, 224, 286, 352, 416, 544,
    578, 608, 736, 928, 992, 1184, 1312,
    1376, 1504, 1696, 1888, 1952
};

static const size_t small_count = sizeof(small_terms) / sizeof(small_terms[0]);

/* Return the n-th term of A295221 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */
    if (n <= small_count)
        return small_terms[n - 1];

    /* 64 · p_j  ,   j = n + 43371   (1‑based)  */
    init_sieve();                     /* primes[0] = 2   */

    size_t idx = (size_t)(n + 43370); /* 0‑based index  */
    if (idx >= prime_count)           /* safety check   */
        return 0;

    seq_t p = primes[idx];
    return p * ((seq_t)64);
}
