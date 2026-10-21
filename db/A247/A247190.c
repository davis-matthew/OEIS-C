/*
 * A247190  Calculate the smallest positive residues of
 *          2!,3!,...,(prime(n)-1)! modulo prime(n) and stop the calculation
 *          at the moment m when for the first time there appear two equal
 *          numbers. If m!==k!, then a(n)=k; but a(n)=0 if no such m exists.
 * https://oeis.org/A247190
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k for which k! ≡ m! (mod prime(n)) first
 *            occurs, where m is the first factorial with a repeated residue.
 *            If no repetition exists, a(n) = 0
 * Range:     u8 n <= 1159, u16 n <= 9340, u32 n <= 9340, u64 n <= 9340,
 *            u128 n <= 9340, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    uint64_t w = 2;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

/* Cache of primes generated on demand.  prime_cache[0] == 2, ... */
static uint32_t *prime_cache = NULL;
static idx_t prime_cnt = 0;

/* Return the k‑th prime (1‑based).  Generates primes as necessary. */
static uint32_t get_prime_by_index(idx_t k)
{
    if (k == 0) return 0;                   /* never used for n ≥ 3 */
    if (prime_cnt < k) {
        uint32_t last = (prime_cnt == 0) ? 1 : prime_cache[prime_cnt - 1];
        uint32_t cand = last + 1;
        while (prime_cnt < k) {
            if (is_prime_u64(cand)) {
                uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                 (prime_cnt + 1) * sizeof(uint32_t));
                if (!tmp) exit(1);        /* out of memory */
                prime_cache = tmp;
                prime_cache[prime_cnt++] = cand;
            }
            cand++;
        }
    }
    return prime_cache[k - 1];
}

/* Main sequence function.  idx_t and seq_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* offset is 3 */

    uint32_t p = get_prime_by_index(n);        /* n‑th prime */
    int32_t *seen = (int32_t *)malloc(p * sizeof(int32_t));
    if (!seen) return (seq_t)0;                /* allocation failure */

    for (uint32_t i = 0; i < p; ++i) seen[i] = -1;

    uint32_t cur = 1;
    for (uint32_t i = 2; i < p; ++i) {
        cur = (uint32_t)(((uint64_t)cur * i) % p);
        if (seen[cur] != -1) {
            seq_t res = (seq_t)seen[cur];
            free(seen);
            return res;
        }
        seen[cur] = (int32_t)i;
    }

    free(seen);
    return (seq_t)0;
}
