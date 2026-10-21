/*
 * A255646  Odd triprimes modulo 10.
 * https://oeis.org/A255646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046316(n) mod 10 (A046316 = odd integers with exactly 3
 *            prime factors counted with multiplicity)
 * Range:     u8 n <= 18, u16 n <= 8824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t *prime_list = NULL;   /* list of primes */
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static idx_t next_prime_candidate = 2;

/* ensure prime_list contains all primes <= limit */
static void ensure_primes_up_to(idx_t limit) {
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p * p > next_prime_candidate) break;
            if (next_prime_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
                idx_t *tmp = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = next_prime_candidate;
        }
        ++next_prime_candidate;
    }
}

/* count total prime factors with multiplicity of n (n is odd) */
static size_t count_prime_factors_omega(idx_t n) {
    size_t cnt = 0;
    idx_t tmp = n;
    idx_t limit = (idx_t)((double)tmp * 0.5);  /* sqrt approx; safe for trimming */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t p = prime_list[i];
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt > 3) return cnt;
        }
    }
    if (tmp > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */
    /* Ensure we have primes up to sqrt of the current search space.
       We'll grow this as needed below. */
    ensure_primes_up_to(13); /* start with some primes */

    idx_t count = 0;
    idx_t candidate = 3;  /* first odd integer > 1 */
    while (true) {
        /* Ensure primes up to sqrt(candidate) */
        idx_t sq = (idx_t)((double)candidate * 0.5);
        ensure_primes_up_to(sq);

        if (count_prime_factors_omega(candidate) == 3) {
            ++count;
            if (count == n) {
                return (seq_t)candidate % (seq_t)10;
            }
        }
        candidate += 2; /* next odd */
    }
}
