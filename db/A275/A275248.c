/*
 * A275248  Sequence of pairwise relatively prime numbers of class P_4 (see
 *          comment in A275246).
 * https://oeis.org/A275248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes[i] * primes[i+2] where i = 4 * floor((n-1)/2) +
 *            ((n-1) mod 2). The primes array contains the natural sequence of
 *            primes. The chosen pairs are disjoint, yielding pairwise coprime
 *            terms
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PRIMES 40000   /* enough for n <= 10000 (need about 20000 primes) */

/* cache of primes */
static uint64_t _primes[MAX_PRIMES];
static size_t _prime_cnt = 0;
static bool _prime_inited = false;

/* generate primes up to the required count using a simple sieve */
static void _ensure_primes(size_t needed)
{
    if (_prime_cnt >= needed)
        return;

    /* rough upper bound for the needed-th prime:
       for n>=6, p_n < n (log n + log log n)
       we take a safety factor of 12 times n */
    size_t limit = needed * 12 + 100;
    if (limit < 100)
        limit = 100;

    size_t *marked = (size_t *)calloc(limit + 1, sizeof(size_t));
    if (!marked)
        return;                 /* on serious failure we let the call fail */

    size_t count = 0;
    for (size_t i = 2; i <= limit && count < needed; ++i) {
        if (!marked[i]) {
            if (count < MAX_PRIMES)
                _primes[count] = i;
            ++count;

            /* mark multiples of i */
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    marked[j] = 1;
            }
        }
    }
    free(marked);
    _prime_cnt = count;
    _prime_inited = true;
}

seq_t a(idx_t n)
{
    /* sequence is 1-indexed */
    if (n == 0)
        return 0;              /* undefined for n<1, return 0 as sentinel */

    /* compute how many primes we need; each term uses two primes,
       so we need about 2*n primes, but some primes are skipped.
       A safe overestimate is 2*n + 4. */
    size_t needed = 2 * (size_t)n + 4;
    _ensure_primes(needed);

    idx_t k = n - 1;                       /* 0‑based term index */
    size_t i = 4 * ((size_t)k / 2) + ((size_t)k % 2);

    seq_t p = (seq_t)_primes[i];
    seq_t q = (seq_t)_primes[i + 2];
    return p * q;
}
