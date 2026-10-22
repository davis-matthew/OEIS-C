/*
 * A362619  One and all numbers whose greatest prime factor is a mode, meaning
 *          it appears at least as many times as each of the others.
 * https://oeis.org/A362619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A362619
   Numbers whose greatest prime factor has multiplicity
   at least as large as every other prime factor.  The
   first several terms are:
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17,
     18, 19, 21, 22, 23, 25, 26, 27, ...
   This implementation generates the sequence by testing integers
   one by one and caching the results so that repeated calls are
   fast.  Only 64‑bit factorisation is used; the sequence values
   needed for the test sizes all fit in 64 bits.  The code
   performs a sieve up to 1 000 000 once and then factors each
   candidate with the pre‑computed primes.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime sieve up to 1 000 000 ---------------------------------------------------- */
static const unsigned int PRIME_SIEVE_LIMIT = 1000000;

static unsigned int prime_list[78500];     /* 78498 primes below 1e6 */
static size_t   prime_count = 0;

/* Initialise the prime list once */
static void init_prime_sieve(void)
{
    if (prime_count > 0) return;          /* already initialised */
    bool *is_comp = (bool*)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_comp) return;                 /* malloc failed – unlikely */
    for (unsigned int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) is_comp[i] = false;

    unsigned int p = 2;
    while (p * p <= PRIME_SIEVE_LIMIT) {
        if (!is_comp[p]) {
            for (unsigned int j = p * p; j <= PRIME_SIEVE_LIMIT; j += p)
                is_comp[j] = true;
        }
        ++p;
    }
    for (unsigned int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) prime_list[prime_count++] = i;
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Checks whether a 64‑bit integer has the required property */
static bool qualifies_uint64(uint64_t n)
{
    if (n == 1) return true;          /* 1 is defined to be in the sequence */

    uint64_t m = n;
    /* Small arrays: factorisation never involves more than ~64 primes */
    uint64_t primes[64];
    int       exps [64];
    size_t    fcnt  = 0;

    init_prime_sieve();               /* ensure primes present */

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        if (m % prime_list[i] == 0) {
            int e = 0;
            while (m % prime_list[i] == 0) {
                m /= prime_list[i];
                ++e;
            }
            primes[fcnt] = prime_list[i];
            exps [fcnt] = e;
            ++fcnt;
        }
    }
    if (m > 1) {                      /* remaining prime factor */
        primes[fcnt] = m;
        exps [fcnt] = 1;
        ++fcnt;
    }

    /* Find the largest prime factor */
    uint64_t largest = 0;
    int      exp_largest = 0;
    size_t   idx_largest = 0;
    for (size_t i = 0; i < fcnt; ++i) {
        if (primes[i] > largest) {
            largest = primes[i];
            exp_largest = exps[i];
            idx_largest = i;
        }
    }
    /* Compare its exponent with exponents of all other primes */
    for (size_t i = 0; i < fcnt; ++i) {
        if (i == idx_largest) continue;
        if (exp_largest < exps[i]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cachable array of the already‑computed terms */
static seq_t *sequence = NULL;          /* array of computed a(k) */
static size_t seq_len  = 0;             /* number of terms stored    */
static size_t seq_cap  = 0;             /* current capacity          */

/* ------------------------------------------------------------------ */
/* Main function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* not used but safe */

    if (seq_len < (size_t)n) {    /* extend cache up to n */
        uint64_t candidate = (seq_len==0)?1: (uint64_t) sequence[seq_len-1] + 1;
        for ( ; seq_len < (size_t)n; ++candidate) {
            if (!qualifies_uint64(candidate)) continue;
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap? seq_cap << 1: 16;
                seq_t *tmp = (seq_t*)realloc(sequence, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* malloc failure – unlikely */
                sequence = tmp;
                seq_cap   = new_cap;
            }
            sequence[seq_len++] = (seq_t)candidate;
        }
    }
    return sequence[n-1];
}
