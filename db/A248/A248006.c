/*
 * A248006  Least positive integer m such that m + n divides phi(m*n), where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A248006
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 603, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = least positive integer m such that
 *  m + n divides phi(m*n)   ,  for n >= 3.
 *  The function computes the value by searching m = 3 … n
 *  and evaluating phi(m*n) using the set of distinct prime
 *  divisors of m and n.  Factorisation up to the current n
 *  is done via a smallest‑prime‑factor sieve built lazily.
 *
 *  Complexity:  O(n · ω(n))  per n, where ω(n) is the number
 *  of distinct prime factors (≤ 4 for n ≤ 10000).  This
 *  easily fits in the 2 s time limit for n up to ~1e4.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static size_t max_n_seen = 0;
static size_t *spf = NULL;          /* smallest prime factor array */
static size_t *prime_list = NULL;   /* list of primes up to current limit */
static size_t prime_cnt   = 0;      /* number of primes stored */

/* Build sieve for primes and SPF up to `limit`.  Rebuild only when
 * a larger limit is requested.  The arrays are allocated on the
 * heap; old ones are freed. */
static void init_spf_and_primes(idx_t limit) {
    size_t len = (size_t)limit + 1;
    size_t *new_spf = (size_t *)calloc(len, sizeof(size_t));
    if (!new_spf) return;  /* allocation failure – unlikely in tests */

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {            /* i is prime */
            new_spf[i] = i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    /* build prime list */
    size_t *new_primes = (size_t *)malloc(len * sizeof(size_t));
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_spf[i] == i)
            new_primes[cnt++] = i;

    /* free old storage */
    free(spf);
    free(prime_list);

    spf = new_spf;
    prime_list = new_primes;
    prime_cnt = cnt;
    max_n_seen = limit;
}

/* Helper: factorise `value` into distinct primes using the global SPF.
 * The primes are written into `out` and the count returned in *cnt.
 * The arrays passed in must be large enough (max ~4 for n <= 10000). */
static void factor_unique(idx_t value, int *out, int *cnt) {
    idx_t x = value;
    int c = 0;
    while (x > 1) {
        int p = (int)spf[(size_t)x];
        out[c++] = p;
        while (x % (idx_t)p == 0)
            x /= (idx_t)p;
    }
    *cnt = c;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;      /* out of defined range */

    /* Ensure SPF and prime list are ready up to n. */
    if (n > max_n_seen)
        init_spf_and_primes(n);

    /* Factor the fixed n once: store its distinct primes. */
    int n_primes[6], n_cnt = 0;
    factor_unique(n, n_primes, &n_cnt);

    /* Search m from 3 to n (a(n) >= 3). */
    for (idx_t m = 3; m <= n; ++m) {
        /* Compute phi(m*n) = m*n * Π_{p|m*n} (1-1/p) */
        seq_t prod = (seq_t)m * (seq_t)n;
        seq_t phi  = prod;

        /* factor m separately */
        int m_primes[6], m_cnt = 0;
        factor_unique(m, m_primes, &m_cnt);

        /* Apply primes from n first. */
        for (int i = 0; i < n_cnt; ++i) {
            int p = n_primes[i];
            phi = phi / (seq_t)p * ((seq_t)p - 1);
        }

        /* Then apply primes from m that are not already used. */
        for (int i = 0; i < m_cnt; ++i) {
            int p = m_primes[i];
            bool found = false;
            for (int j = 0; j < n_cnt; ++j)
                if (n_primes[j] == p) { found = true; break; }
            if (!found) {
                phi = phi / (seq_t)p * ((seq_t)p - 1);
            }
        }

        /* Test the divisibility condition */
        if (phi % (m + n) == 0) {
            return (seq_t)m;
        }
    }

    /* Fallback – should never happen, but return n. */
    return (seq_t)n;
}
