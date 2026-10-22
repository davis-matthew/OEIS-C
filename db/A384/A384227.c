/*
 * A384227  Smaller members p of sexy prime pairs (p,p+6) such that both p and
 *          p+6 are emirps.
 * https://oeis.org/A384227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that p and p+6 are prime, both are
 *            emirps (different from their decimal reversal which is also
 *            prime)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 23, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

/* Memoized list of sequence values. */
static seq_t *results = NULL;
static size_t res_size = 0;      /* number of stored terms in `results`. */
static idx_t current_limit = 0;  /* upper bound of the sieve in the last rebuild. */

/* Rebuild the results list up to at least `need` terms by extending
   the sieve to a larger limit. */
static void extend_and_recompute(idx_t need)
{
    if (current_limit == 0) current_limit = 100000;   /* start with a sane limit */

    /* Increase the limit until we have enough terms. */
    while (res_size < (size_t)need) {
        current_limit *= 2;   /* exponential growth */

        /* Allocate and run a classic Eratosthenes sieve up to `current_limit`. */
        uint8_t *isPrime = (uint8_t *)malloc((size_t)current_limit + 1);
        if (!isPrime) abort();
        memset(isPrime, 1, (size_t)current_limit + 1);
        isPrime[0] = isPrime[1] = 0;

        for (idx_t i = 2; i * i <= current_limit; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= current_limit; j += i)
                    isPrime[j] = 0;
            }
        }

        /* Rebuild the results list from scratch. */
        free(results);        /* discard old list */
        results = NULL;
        res_size = 0;

        /* Rough capacity guess; will grow if needed. */
        size_t cap = (size_t)(current_limit / 20 + 1);
        results = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!results) abort();

        for (idx_t p = 2; p + 6 <= current_limit; ++p) {
            if (!isPrime[p] || !isPrime[p + 6]) continue;

            seq_t pp = (seq_t)p;
            seq_t qp = pp + 6;

            seq_t rp = reverse_decimal(pp);
            seq_t rq = reverse_decimal(qp);

            /* Skip if either number is non‑emirp:
               - it's a palindrome (reverse equals itself),
               - or its reverse is not prime. */
            if (rp == pp || !is_prime_big(rp)) continue;
            if (rq == qp || !is_prime_big(rq)) continue;

            /* Append pp to results. */
            if (res_size == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(results, cap * sizeof(seq_t));
                if (!tmp) abort();
                results = tmp;
            }
            results[res_size++] = pp;
        }
        free(isPrime);
    }
}

/* Sequence function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for 0 is irrelevant */

    if (res_size < (size_t)n) extend_and_recompute(n);
    return results[n - 1];    /* convert 1‑based index to 0‑based array index */
}
