/*
 * A246375  Permutation of natural numbers: a(1) = 1, a(2n) = 2*a(n), a(2n+1)
 *          = A003961(1+a(n)). [Where A003961(n) shifts the prime
 *          factorization of n one step towards larger primes].
 * https://oeis.org/A246375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2*n) = 2*a(n), a(2*n+1) = A003961(1 + a(n)) A003961
 *            shifts each prime factor of its argument to the next prime
 * Range:     u8 n <= 46, u16 n <= 558, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   floor_sqrt, grow_seq, is_prime_small64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h" /* floor_sqrt(seq_t) */
#include "../utils/grow_seq.h"   /* grow_seq(seq_t **, size_t *, size_t) */
#include "../utils/is_prime_small64.h"

/*** prime cache ***/
static seq_t *prime_cache = NULL;   /* list of known primes (seq_t) */
static size_t prime_cap   = 0;      /* allocated capacity of prime_cache */
static size_t prime_cnt   = 0;      /* number of primes stored */

/* ensure prime_cache contains all primes <= limit */
static void ensure_primes_upto(seq_t limit)
{
    while (prime_cnt == 0 || prime_cache[prime_cnt - 1] < limit) {
        seq_t cand = (prime_cnt == 0 ? (seq_t)2 : prime_cache[prime_cnt - 1] + (seq_t)1);
        while (!is_prime_small64(cand)) cand = cand + (seq_t)1;
        grow_seq(&prime_cache, &prime_cap, prime_cnt + 1);
        prime_cache[prime_cnt++] = cand;
    }
}

/* next prime after p (p >= 2) */
static seq_t next_prime(seq_t p)
{
    seq_t q = p + (seq_t)1;
    while (!is_prime_small64(q)) q = q + (seq_t)1;
    return q;
}

/* A003961: shift prime factorisation of n towards larger primes */
static seq_t a003961(seq_t n)
{
    if (n <= (seq_t)1) return (seq_t)1; /* by definition */

    seq_t temp   = n;
    seq_t result = (seq_t)1;

    while (temp > (seq_t)1) {
        seq_t sqrt_temp = floor_sqrt(temp);
        ensure_primes_upto(sqrt_temp);

        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = prime_cache[i];
            if (p > sqrt_temp) break;
            if (temp % p == (seq_t)0) {
                /* factor p^e */
                uint64_t e = 0;
                while (temp % p == (seq_t)0) {
                    temp = temp / p;
                    ++e;
                }
                seq_t next_p = next_prime(p);
                seq_t pow_val = (seq_t)1;
                for (uint64_t j = 0; j < e; ++j) pow_val = pow_val * next_p;
                result = result * pow_val;
            }
        }

        /* remaining prime factor larger than sqrt_temp (if any) */
        if (temp > (seq_t)1) {
            seq_t next_p = next_prime(temp);
            result = result * next_p;
            temp = (seq_t)1;
        }
    }
    return result;
}

/* memoised a(n) */
static seq_t *a_cache   = NULL;   /* cache array (index 0 unused) */
static idx_t a_cached_max = (idx_t)0;
static size_t a_cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for 0, return guard */

    if (n <= a_cached_max) return a_cache[n];

    size_t need = (size_t)n + (size_t)1; /* allocate up to index n */
    if (need > a_cache_cap) {
        grow_seq(&a_cache, &a_cache_cap, need);
    }

    for (idx_t k = a_cached_max + (idx_t)1; k <= n; ++k) {
        if (k == (idx_t)1) {
            a_cache[1] = (seq_t)1;
        } else if ((k & (idx_t)1) == (idx_t)0) { /* even */
            a_cache[k] = (seq_t)2 * a_cache[k / (idx_t)2];
        } else { /* odd */
            seq_t val = a_cache[k / (idx_t)2] + (seq_t)1;
            a_cache[k] = a003961(val);
        }
    }

    a_cached_max = n;
    return a_cache[n];
}
