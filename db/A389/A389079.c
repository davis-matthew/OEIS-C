/*
 * A389079  Odd nonsquarefree numbers k such that {sum of unitary divisors of
 *          k} plus {sum of squarefree divisors of k} >= 2*k.
 * https://oeis.org/A389079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Sequence A389079 (odd nonsquarefree numbers satisfying a divisor sum inequality).
   The function `a` searches successive odd integers, uses a prime sieve for factorisation,
   and records those that fulfil the conditions until the n‑th term is found.  */

/* -------------------------------------------------------------------------- */
/* Local helper: generate all primes up to a given limit (inclusive).         */
/* -------------------------------------------------------------------------- */
static void
generate_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (*primes_out != NULL) return; /* already built */

    size_t lim = (size_t)limit;
    /* Sieve of Eratosthenes: use a simple boolean array. */
    bool *is_prime = (bool *)malloc((lim + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= lim; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= lim; ++p) {
        if (is_prime[p]) {
            for (size_t multiple = p * p; multiple <= lim; multiple += p)
                is_prime[multiple] = false;
        }
    }

    /* Count primes and store them. */
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) primes[idx++] = (uint64_t)i;

    free(is_prime);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* -------------------------------------------------------------------------- */
/* Main function: returns the n-th term of the sequence.                      */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static state for memoised terms and primes. */
    static seq_t  *terms      = NULL;   /* collected sequence values          */
    static size_t terms_cap   = 0;      /* current allocated capacity         */
    static size_t terms_cnt   = 0;      /* number of terms stored             */
    static idx_t  last_checked = 1;     /* last odd number examined           */

    static uint64_t *primes   = NULL;   /* list of primes (uint64_t)          */
    static size_t   prime_cnt = 0;      /* number of primes                   */

    /* Build the prime list once (enough for factors of expected k values). */
    if (!primes) {
        const idx_t LIMIT = 2000000ULL;  /* ~2 million, more than enough */
        generate_primes_upto(LIMIT, &primes, &prime_cnt);
    }

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < n) {
        last_checked += 2;                     /* next odd number                */
        idx_t k_idx = last_checked;            /* idx_t representation of k       */
        seq_t k = (seq_t)k_idx;                /* cast to seq_t for arithmetic    */

        /* Factor k and compute the unitary and squarefree divisor sums. */
        idx_t temp = k_idx;
        bool nonsqfree = false;
        seq_t U = 1;   /* product of (1 + p^e) for each prime factor p^e */
        seq_t S = 1;   /* product of (1 + p) across distinct primes      */

        for (size_t pi = 0; pi < prime_cnt && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= temp; ++pi) {
            idx_t p = primes[pi];
            if (temp % p == 0) {
                idx_t e = 0;
                seq_t p_pow = 1;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                    p_pow *= (seq_t)p;
                }
                if (e >= 2) nonsqfree = true;
                U *= (seq_t)1 + p_pow;
                S *= (seq_t)1 + (seq_t)p;
            }
        }

        if (temp > 1) {                        /* remaining prime factor (exponent 1) */
            U *= (seq_t)1 + (seq_t)temp;
            S *= (seq_t)1 + (seq_t)temp;
        }

        if (!nonsqfree) continue;              /* skip square‑free numbers */

        /* Check inequality U(k)+S(k) >= 2*k. */
        seq_t lhs = U + S;
        seq_t rhs = (seq_t)2 * k;
        if (lhs >= rhs) {
            /* Store the term, growing capacity if necessary. */
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap == 0 ? 64 : terms_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) exit(1);
                terms      = new_terms;
                terms_cap  = new_cap;
            }
            terms[terms_cnt++] = k;
        }
    }

    return terms[n - 1];  /* 1‑based indexing per OEIS */
}
