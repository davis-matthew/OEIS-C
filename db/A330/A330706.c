/*
 * A330706  Numbers m such that the prime factorization of m! contains no
 *          composite exponents.
 * https://oeis.org/A330706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   _make_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330706: numbers m such that every exponent in the prime factorisation
 * of m! is 1 or a prime.
 *
 * The function builds the sequence by checking each integer m successively,
 * storing the qualifying terms, and returning the n‑th term (1‑based).
 * The sequence is finite; terms beyond the last one are returned as 0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_make_primes.h"

/* --------------------------------------------------------------------
 * Helper: test small integer primality (exponents are small).
 * -------------------------------------------------------------------- */
static bool _is_prime_uint64(uint64_t x)
{
    if (x < 2u) return false;
    if (x == 2u) return true;
    if ((x & 1u) == 0u) return false;
    for (uint64_t d = 3u; d * d <= x; d += 2u)
        if (x % d == 0u) return false;
    return true;
}

/* --------------------------------------------------------------------
 * Helper: test whether m! satisfies the condition.
 * m is idx_t (uint64_t).  primes[] contains all primes ≤ limit.
 * -------------------------------------------------------------------- */
static bool _valid_m(idx_t m, uint64_t *primes, size_t prime_cnt)
{
    for (size_t i = 0; i < prime_cnt && primes[i] <= m; ++i) {
        idx_t p = primes[i];
        idx_t e = 0;
        idx_t t = m / p;
        while (t) {
            e += t;
            t /= p;
        }
        if (e == 1) continue;
        if (_is_prime_uint64(e)) continue;
        return false;
    }
    return true;
}

/* --------------------------------------------------------------------
 * Global static data
 * -------------------------------------------------------------------- */
static seq_t *terms = NULL;       /* stored sequence terms */
static size_t term_cnt = 0;       /* number of terms stored */
static size_t term_cap = 0;       /* capacity of terms array */

static uint64_t *prime_arr = NULL;/* array of primes */
static size_t prime_cnt = 0;      /* number of primes */
static const idx_t PRIME_LIMIT = 200; /* enough for all possible m */

/* --------------------------------------------------------------------
 * Grow the terms array if needed.
 * -------------------------------------------------------------------- */
static void _grow_terms(void)
{
    if (term_cnt < term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    term_cap = new_cap;
}

/* --------------------------------------------------------------------
 * Pre‑compute all terms once.  The sequence is finite, so a fixed
 * upper bound (200) is sufficient to find all entries.
 * -------------------------------------------------------------------- */
static void _init_terms(void)
{
    if (terms) return;          /* already initialised */
    term_cap = 16;
    terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    if (!terms) abort();

    _make_primes(PRIME_LIMIT, &prime_arr, &prime_cnt);

    for (idx_t m = 1; m <= PRIME_LIMIT; ++m) {
        if (_valid_m(m, prime_arr, prime_cnt)) {
            _grow_terms();
            terms[term_cnt++] = (seq_t)m;
        }
    }
}

/* --------------------------------------------------------------------
 * Required function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    _init_terms();                 /* ensure terms are ready */

    if (n - 1 < term_cnt)
        return terms[n - 1];
    return (seq_t)0;               /* beyond the last known term */
}
