/*
 * A338756  Number of consecutive triples of quadratic residues mod p in the
 *          interval [1..p-1] where p is the n-th prime.
 * https://oeis.org/A338756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive triples of quadratic residues
 *            modulo the n-th prime
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Simple prime generation up to a given limit (inclusive).  */
static void generate_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) {
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)malloc(sz);
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    size_t c = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++c;
    }
    uint64_t *pr = (uint64_t*)malloc(c * sizeof(uint64_t));
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            pr[k++] = (uint64_t)i;
        }
    }
    *out = pr;
    *cnt = c;
    free(is_prime);
}

/* Cache of primes */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for offset < 1 */

    /* Ensure sufficient primes cached for the n-th prime. */
    if (prime_cnt < (size_t)n) {
        idx_t new_limit = n * 20;
        if (new_limit < 20) new_limit = 20;
        while (true) {
            uint64_t *tmp;
            size_t cnt;
            generate_primes_upto(new_limit, &tmp, &cnt);
            if (primes) free(primes);
            primes = tmp;
            prime_cnt = cnt;
            prime_limit = new_limit;
            if (prime_cnt >= (size_t)n) break;
            new_limit *= 2;
        }
    }

    uint64_t p_u64 = primes[n - 1];
    idx_t p = (idx_t)p_u64;

    if (p <= 3) return 0;         /* no possible triple */

    /* Mark quadratic residues modulo p. */
    unsigned char *residue = (unsigned char*)malloc((size_t)p);
    memset(residue, 0, (size_t)p);
    for (idx_t a = 1; a <= (p - 1) / 2; ++a) {
        idx_t sq = ((uint64_t)a * a) % p;
        residue[sq] = 1;
    }

    /* Count consecutive triples of residues. */
    seq_t count = 0;
    for (idx_t k = 1; k <= p - 3; ++k) {
        if (residue[k] && residue[k + 1] && residue[k + 2]) {
            ++count;
        }
    }

    free(residue);
    return count;
}
