/*
 * A322368  Heinz numbers of disconnected integer partitions.
 * https://oeis.org/A322368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz number of the n-th disconnected * integer
 *            partition, in increasing order of the Heinz * number. * Sequence
 *            elements are the products of the primes at the * part sizes of
 *            every integer partition that is disconnected * according to the
 *            prime‑factorisation hypergraph. * The first term i...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* global state ------------------------------------------------------ */
static bool initialized = false;
static idx_t cur_m = 2;                /* next integer to test          */
static size_t terms_size = 0;          /* number of terms cached        */
static size_t terms_cap  = 0;          /* allocated capacity            */
static seq_t *terms = NULL;           /* array of cached terms          */

static const int PRIME_LIMIT = 500000; /* enough for 10 000 terms         */
static int  *primes      = NULL;       /* list of primes up to limit     */
static size_t primes_cnt = 0;          /* number of primes               */
static int  *prime_index = NULL;       /* mapping prime value -> index   */

/* ------------------------------------------------------------------ */
/* Euclidean algorithm for indices (uint64_t)                          */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Allocate or grow the terms array (handles C and C++).               */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap >= need)
        return;

    size_t newcap = terms_cap ? terms_cap << 1 : 8;
    while (newcap < need)
        newcap <<= 1;

#ifdef __cplusplus
    seq_t *newptr = new seq_t[newcap];
    if (terms) {
        for (size_t i = 0; i < terms_size; ++i)
            newptr[i] = terms[i];
        delete[] terms;
    }
    terms = newptr;
#else
    seq_t *newptr = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!newptr)
        exit(1);    /* allocation failure */
    terms = newptr;
#endif
    terms_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes: populate primes[] and prime_index[].       */
static void init_primes(void)
{
    prime_index = (int *)calloc(PRIME_LIMIT + 1, sizeof(int));
    if (!prime_index)
        exit(1);

    /* allocate a moderate prime array; 60000 > number of primes <= 500000 */
    primes = (int *)malloc(sizeof(int) * 60000);
    if (!primes)
        exit(1);

    unsigned char *sieve = (unsigned char *)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve)
        exit(1);

    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) {
            primes[primes_cnt++] = i;
            prime_index[i] = (int)(primes_cnt); /* 1‑based index */
            if ((long long)i * i <= PRIME_LIMIT)
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = 1;
        }

    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Perform one pass of the sequence generation until n terms are   */
/* available.                                                         */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        /* initialise data structures */
        init_primes();
        ensure_terms_cap(1);         /* space for a(1) */
        terms[0] = (seq_t)1;         /* a(1) = 1 */
        terms_size = 1;
        cur_m = 2;                   /* start testing from 2 */
        initialized = true;
    }

    /* generate terms while needed */
    while (terms_size < (size_t)n) {
        /* evaluate integer cur_m */
        idx_t tmp = cur_m;
        int indices[32];
        size_t idx_len = 0;

        /* factorisation via trial division */
        for (size_t pi = 0; pi < primes_cnt && (idx_t)primes[pi] * primes[pi] <= tmp; ++pi) {
            int p = primes[pi];
            if (tmp % p == 0) {
                size_t exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                }
                for (size_t e = 0; e < exp; ++e) {
                    if (idx_len < sizeof(indices)/sizeof(indices[0]))
                        indices[idx_len++] = prime_index[p];
                }
            }
        }
        if (tmp > 1) {                /* remaining prime factor */
            int p = (int)tmp;
            if (idx_len < sizeof(indices)/sizeof(indices[0]))
                indices[idx_len++] = prime_index[p];
        }

        /* check disconnectedness condition */
        if (idx_len >= 2) {
            bool good = false;
            for (size_t i = 0; i < idx_len && !good; ++i) {
                bool coprime_to_all = true;
                for (size_t j = 0; j < idx_len; ++j) {
                    if (i == j)
                        continue;
                    if (gcd_idx(indices[i], indices[j]) != 1) {
                        coprime_to_all = false;
                        break;
                    }
                }
                if (coprime_to_all)
                    good = true;
            }
            if (good) {
                ensure_terms_cap(terms_size + 1);
                terms[terms_size++] = (seq_t)cur_m;
            }
        }

        ++cur_m;        /* advance to next integer */
    }
}

/* ------------------------------------------------------------------ */
/* Public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    ensure_terms(n);
    return terms[n - 1];
}
