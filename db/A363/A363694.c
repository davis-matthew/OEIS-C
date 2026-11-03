/*
 * A363694  Number of edges in the prime (Gruenberg-Kegel) graph of the
 *          symmetric group, S_n, on n elements.
 * https://oeis.org/A363694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ unordered prime pairs (p < q) : p + q ≤ n }
 *               (number of edges in the prime graph of the symmetric group Sₙ)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Generate all primes up to limit (inclusive) using a simple sieve.
 * The caller must free the returned array. */
static void _sieve_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }

    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool*)calloc(sz, sizeof(bool));
    if (!is_composite) {
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }

    /* rough initial capacity: about half of limit + 1, enough for most cases */
    size_t cap = (size_t)(limit / 2 + 1);
    uint64_t *primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        *primes_out = NULL;
        *cnt_out    = 0;
        return;
    }
    size_t pn = 0;

    for (size_t p = 2; p <= (size_t)limit; ++p) {
        if (!is_composite[p]) {
            if (pn == cap) {                 /* grow array if needed */
                cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) {
                    free(primes);
                    free(is_composite);
                    *primes_out = NULL;
                    *cnt_out    = 0;
                    return;
                }
                primes = tmp;
            }
            primes[pn++] = (uint64_t)p;

            if (p * p <= (size_t)limit) {     /* mark multiples */
                for (uint64_t q = (uint64_t)p * (uint64_t)p; q <= (uint64_t)limit; q += p) {
                    is_composite[(size_t)q] = true;
                }
            }
        }
    }

    /* shrink array to actual size */
    uint64_t *tmp = (uint64_t*)realloc(primes, pn * sizeof(uint64_t));
    if (tmp) primes = tmp;

    free(is_composite);
    *primes_out = primes;
    *cnt_out    = pn;
}

seq_t a(idx_t n)
{
    /* Generate all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_count = 0;
    _sieve_primes_upto(n, &primes, &prime_count);

    if (!primes || prime_count < 2) {
        /* No distinct pairs possible. */
        return 0;
    }

    /* Count unordered prime pairs (p < q) such that p + q ≤ n
       using the two‑pointer technique. */
    seq_t count = 0;
    size_t i = 0;
    size_t j = prime_count - 1;

    while (i < j) {
        /* primes[i] + primes[j] <= n  <=> primes[i] <= n - primes[j] */
        if ((idx_t)primes[i] <= n - primes[j]) {
            count += (seq_t)(j - i);   /* all pairs with first prime primes[i] */
            ++i;
        } else {
            --j;
        }
    }

    free(primes);
    return count;
}
