/*
 * A367496  Semiprimes k such that 2*k - 1 and 2*k + 1 are also semiprimes.
 * https://oeis.org/A367496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367496: a(n) = the n‑th semiprime k such that 2*k‑1 and 2*k+1 are also semiprimes. */
/* The implementation builds a prime list once and then searches for terms. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Prime table – built once, lazily                           */
/* ------------------------------------------------------------ */
static uint32_t *primes = NULL;   /* array of primes */
static size_t prime_cnt = 0;      /* number of primes */

/* ---- small sieve up to LIMIT -------------------------------- */
static bool init_primes(void)
{
    if (primes != NULL) return true;                /* already built */

    /* choose a limit large enough for the expected k values.
       1e6 gives primes up to ~78498 primes, sufficient for k up to ~1e12. */
    const uint32_t LIMIT = 1000000;
    bool *is_composite = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_composite) return false;
    memset(is_composite, 0, (LIMIT + 1) * sizeof(bool));
    is_composite[0] = is_composite[1] = true;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = true;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) ++cnt;

    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) { free(is_composite); return false; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) primes[idx++] = i;

    prime_cnt = cnt;
    free(is_composite);
    return true;
}

/* ------------------------------------------------------------ */
/* Semiprime test for uint64_t                                  */
/* ------------------------------------------------------------ */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false; /* smallest semiprime 2*2=4 */

    size_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        if (n % p == 0) {
            ++count;
            n /= p;
            if (n % p == 0) {      /* p^2 factor */
                ++count;
                n /= p;
            }
            if (count > 2) return false;
        }
    }
    if (n > 1) ++count;   /* remaining prime factor */
    return count == 2;
}

/* ------------------------------------------------------------ */
/* Storage for the sequence                                    */
/* ------------------------------------------------------------ */
static seq_t *terms        = NULL;   /* array of found terms */
static size_t term_len     = 0;     /* number of stored terms   */
static size_t term_cap     = 0;     /* allocated capacity      */
static uint64_t last_used  = 0;     /* largest k that has been examined */

/* ------------------------------------------------------------ */
/* Extend the sequence until it has at least target terms.     */
/* ------------------------------------------------------------ */
static void extend_to(idx_t target)
{
    if (!init_primes()) return;            /* cannot proceed without primes */

    while ((idx_t)term_len < target) {
        ++last_used;                       /* next candidate k */

        if (!is_semiprime(last_used)) continue;

        uint64_t m1 = 2 * last_used - 1;
        uint64_t m2 = 2 * last_used + 1;

        if (!is_semiprime(m1) || !is_semiprime(m2)) continue;

        /* store the term */
        if (term_len == term_cap) {
            size_t new_cap = term_cap == 0 ? 64 : term_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) return;             /* allocation failed */
            terms = tmp;
            term_cap = new_cap;
        }
        terms[term_len++] = (seq_t)last_used;
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the problem statement           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is 1‑based; behaviour for 0 is undefined */

    extend_to(n);
    if ((idx_t)term_len >= n) return terms[n - 1];
    return 0;               /* should not happen */
}
