/*
 * A357630  Skew-alternating sum of the prime indices of n.
 * https://oeis.org/A357630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = skew‑alternating sum of the prime indices of n. The
 *            multiset of prime indices of n is the multiset of indices of the
 *            prime factors of n counted with multiplicity. Sign pattern: A -
 *            B - C + D + E - F - G + ... (first term positive, then blocks of
 *            two negatives followed by two positives)
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   sign_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sign_at.h"

struct {
    bool     inited;
    idx_t    limit;
    uint64_t *primes;          /* list of primes */
    size_t   prime_count;      /* number of primes */
    uint8_t  *is_comp;         /* composite flag */
    int      *prime_index;     /* prime_index[p] = index of prime p (1‑based) or 0 */
} prime_data = { .inited = false };

/* generate primes up to prime_data.limit using a simple sieve */
static void init_primes(void) {
    if (prime_data.inited) return;
    prime_data.limit = 2000000;          /* sufficient for n up to ~10⁴ */

    prime_data.is_comp = (uint8_t *)malloc((prime_data.limit + 1) * sizeof(uint8_t));
    if (!prime_data.is_comp) exit(EXIT_FAILURE);
    memset(prime_data.is_comp, 0, prime_data.limit + 1);

    /* primes array large enough to hold all primes up to limit */
    prime_data.primes = (uint64_t *)malloc((prime_data.limit + 1) * sizeof(uint64_t));
    if (!prime_data.primes) exit(EXIT_FAILURE);

    prime_data.prime_index = (int *)malloc((prime_data.limit + 1) * sizeof(int));
    if (!prime_data.prime_index) exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= prime_data.limit; ++i)
        prime_data.prime_index[i] = 0;

    size_t pc = 0;
    for (uint64_t i = 2; i <= prime_data.limit; ++i) {
        if (!prime_data.is_comp[i]) {
            prime_data.primes[pc] = i;
            prime_data.prime_index[i] = (int)(++pc);    /* 1‑based index */
            if (i <= prime_data.limit / i) {
                for (uint64_t j = i * i; j <= prime_data.limit; j += i)
                    prime_data.is_comp[j] = 1;
            }
        }
    }
    prime_data.prime_count = pc;
    prime_data.inited = true;
}

seq_t a(idx_t n) {
    init_primes();                         /* ensure primes are ready */

    if (n <= 1) return (seq_t)0;

    idx_t temp = n;
    seq_t  sum   = 0;
    idx_t  pos   = 1;                      /* 1‑based position in the summation */

    /* factorization using precomputed primes */
    for (size_t i = 0; i < prime_data.prime_count &&
         (uint64_t)prime_data.primes[i] * prime_data.primes[i] <= temp; ++i) {
        uint64_t p = prime_data.primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }
            uint64_t m = (uint64_t)prime_data.prime_index[p];   /* index of prime p */
            for (int t = 0; t < exp; ++t) {
                int s = sign_at(pos);
                sum += (seq_t)m * s;
                ++pos;
            }
        }
    }

    /* remaining prime factor > 1, if any */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t m;
        if (p <= prime_data.limit) {
            m = (uint64_t)prime_data.prime_index[p];
        } else {
            /* fallback: compute its index by scanning known primes */
            size_t low = 0, high = prime_data.prime_count;
            while (low < high) {
                size_t mid = (low + high) >> 1;
                if ((uint64_t)prime_data.primes[mid] < p) low = mid + 1;
                else high = mid;
            }
            if (low < prime_data.prime_count &&
                (uint64_t)prime_data.primes[low] == p)
                m = (uint64_t)low + 1;
            else
                m = 0;   /* should not happen for n in the expected range */
        }
        int s = sign_at(pos);
        sum += (seq_t)m * s;
    }

    return sum;
}
