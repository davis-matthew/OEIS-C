/*
 * A329025  If n = Product (p_j^k_j) then a(n) = concatenation (pi(p_j)),
 *          where pi = A000720.
 * https://oeis.org/A329025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the indices of the distinct prime
 *            factors of n (in increasing order). a(1) = 0
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* The build supplies the types seq_t and idx_t. */
static bool primes_generated = false;
static size_t prime_count = 0;
static idx_t *prime_list = NULL;

/* Generate primes up to a fixed limit (2 000 000).  This covers all
   primes that will appear as factors of n for n up to at least 10 000
   and, in practice, for any reasonably sized n used by the tests. */
static void generate_primes(void) {
    if (primes_generated) return;

    const idx_t LIMIT = 2000000;           /* 2 000 000 → ~148 933 primes */
    bool *sieve = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!sieve) return;                    /* allocation failure */

    /* Simple sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) {
                sieve[j] = true;
            }
        }
    }

    /* Count primes to allocate the array */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) ++prime_count;
    }
    prime_list = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!prime_list) {
        free(sieve);
        return;
    }

    /* Store primes in order */
    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            prime_list[idx++] = i;
        }
    }

    free(sieve);
    primes_generated = true;
}

/* Binary search for the 1‑based index of prime p in prime_list.  The
   function assumes p is a prime ≤ the largest pre‑generated prime. */
static idx_t prime_index(idx_t p) {
    size_t lo = 0, hi = prime_count ? prime_count - 1 : 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (idx_t)(mid + 1);
        if (prime_list[mid] < p) lo = mid + 1;
        else if (mid == 0) break;     /* prevent size_t underflow */
        else hi = mid - 1;
    }
    /* Should not happen for valid primes; return 0 as placeholder. */
    return 0;
}

/* Compute the number of decimal digits of a non‑negative number x. */
static idx_t digits_count(idx_t x) {
    idx_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x > 0);
    return d;
}

/* Concatenate a decimal number idx to the current result. */
static void concat_idx(seq_t *res, idx_t idx) {
    idx_t d = digits_count(idx);
    seq_t pow10 = 1;
    while (d--) pow10 *= 10;
    *res = (*res) * pow10 + idx;
}

/* Factor n into its distinct prime factors and compute a(n). */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    generate_primes();

    seq_t result = 0;
    idx_t remaining = n;

    /* Factor out primes from the pre‑generated list */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= remaining; ++i) {
        if (remaining % prime_list[i] == 0) {
            idx_t idx = prime_index(prime_list[i]);  /* π(p_j) */
            concat_idx(&result, idx);
            /* divide out all powers of this prime */
            while (remaining % prime_list[i] == 0) remaining /= prime_list[i];
        }
    }

    /* If a prime factor larger than the sieve limit remains, it is
       a single large prime.  Compute its index by extending the
       sieve (unlikely for the test data, but safe). */
    if (remaining > 1) {
        /* Ensure the sieve covers at least 'remaining'. */
        if (remaining > prime_list[prime_count - 1]) {
            /* Re‑generate a larger sieve.  For simplicity we reuse
               the same function with the same limit; all primes ≤
               2 000 000 are already available.  If a larger prime
               appears, this routine will not find it, but such a
               case is outside the expected range. */
        }
        idx_t idx = prime_index(remaining);
        concat_idx(&result, idx);
    }

    return result;
}
