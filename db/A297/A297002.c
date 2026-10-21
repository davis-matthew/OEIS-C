/*
 * A297002  Completely multiplicative with a(prime(k)) = prime(2 * k) (where
 *          prime(k) denotes the k-th prime).
 * https://oeis.org/A297002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 728, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A297002 – completely multiplicative with a(prime(k)) = prime(2*k) */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static uint64_t *primes = NULL;          /* list of primes (0‑based)                     */
static size_t    prime_count = 0;        /* number of primes in the list                 */
static int32_t  *prime_index = NULL;     /* prime_index[p] = 1‑based index of prime p     */
static int       primes_initialized = 0; /* flag for one‑time initialization             */

/* Initialise primes and prime_index up to a safe upper bound.
   The bound guarantees that for any n ≤ 10 000 we have all primes
   needed to compute a(p) for prime factors of n. */
static void init_primes(void)
{
    if (primes_initialized) return;

    /* upper bound: enough for prime(2 * π(10000)) ≈ prime(2458) < 200 000   */
    const idx_t limit = (idx_t)200000;

    build_primes_upto(limit, &primes, &prime_count);

    prime_index = (int32_t*)malloc((size_t)(limit + 1) * sizeof(int32_t));
    for (size_t i = 0; i <= (size_t)limit; ++i) prime_index[i] = -1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p <= (uint64_t)limit) {
            prime_index[p] = (int32_t)(i + 1);   /* 1‑based index */
        }
    }

    primes_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;   /* a(1) = 1 */

    init_primes();

    seq_t result = (seq_t)1;
    uint64_t temp   = (uint64_t)n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            int32_t idx = prime_index[p];
            if (idx <= 0) continue;   /* should not happen */
            uint64_t ap  = primes[(size_t)(2 * idx) - 1];   /* a(p) */
            seq_t term   = seq_pow((seq_t)ap, (idx_t)exp);
            result = seq_mul(result, term);
        }
    }

    if (temp > 1) {   /* remaining prime factor > sqrt(original n) */
        uint64_t p = temp;
        int32_t idx = prime_index[p];
        if (idx > 0) {
            uint64_t ap = primes[(size_t)(2 * idx) - 1];
            result = seq_mul(result, (seq_t)ap);   /* exp = 1 */
        }
    }

    return result;
}
