/*
 * A387113  Numbers whose prime indices do not have (strictly) choosable
 *          initial intervals.
 * https://oeis.org/A387113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387113 – Numbers whose prime indices do not have (strictly) choosable initial intervals.
 * The sequence is defined by the property that for a number k, the multiset of prime indices
 * (counted with multiplicity) of k yields initial intervals that are NOT choosable
 * (Hall's condition fails).  The function below generates terms on demand by
 * testing each integer in increasing order until the requested index is found.
 *
 * Complexity: O(n * sqrt(m)) for the n-th term, where m is the value of the term.
 * For n ≤ 10 000 this is comfortably within the time limits.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
/* Prime sieve (static small upper bound)                       */
/* ------------------------------------------------------------ */
#define PRIME_LIMIT 5000000U   /* sufficient for the range of terms needed */

static uint32_t *primes = NULL;   /* primes[0] = 2, ... */
static size_t   prime_count = 0;  /* number of primes */

static void init_primes(void)
{
    if (primes != NULL) return;     /* already initialized */

    unsigned char *is_prime = (unsigned char*)malloc(PRIME_LIMIT + 1);
    if (!is_prime) return;           /* allocation failure → no primes */

    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;

    /* count primes */
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    primes = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }

    size_t pos = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[pos++] = (uint32_t)i;

    free(is_prime);
    /* primes are sorted ascending */
}

static size_t prime_index_of(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_count && primes[lo] == p) ? (lo + 1) : 0;
}

/* ------------------------------------------------------------ */
/* Cache of computed terms                                       */
/* ------------------------------------------------------------ */
static idx_t      next_candidate = 1;  /* next integer to be tested (1‑based)   */
static seq_t    *terms = NULL;        /* cached terms array                    */
static size_t    terms_cap = 0;       /* allocated capacity of terms array     */
static size_t    terms_cnt = 0;       /* number of terms stored                */

static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (tmp) { terms = tmp; terms_cap = new_cap; }
}

/* ------------------------------------------------------------ */
/* Test whether an integer is in the sequence                   */
/* ------------------------------------------------------------ */
static bool in_sequence(idx_t num)
{
    uint64_t n = (uint64_t)num;
    if (n < 2) return false;          /* 1 is not in the sequence */

    /* collect prime indices, counted with multiplicity */
    uint32_t indices[64];
    size_t   m = 0;

    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
                if (m < 64) indices[m++] = (uint32_t)prime_index_of(p);
            }
        }
    }
    if (temp > 1) {  /* remaining prime factor larger than sqrt */
        uint32_t p = (uint32_t)temp;
        if (m < 64) indices[m++] = (uint32_t)prime_index_of(p);
    }

    if (m == 0) return false;         /* no prime indices → choosable */

    /* insertion sort of indices (small array) */
    for (size_t i = 1; i < m; ++i) {
        uint32_t key = indices[i];
        size_t j = i;
        while (j > 0 && indices[j-1] > key) {
            indices[j] = indices[j-1];
            --j;
        }
        indices[j] = key;
    }

    /* Hall's condition: find j with indices[j-1] < j (1‑based) */
    for (size_t j = 1; j <= m; ++j)
        if ((seq_t)indices[j-1] < (seq_t)j)
            return true;   /* not choosable → number is in the sequence */

    return false;  /* choosable → not in the sequence */
}

/* ------------------------------------------------------------ */
/* Public sequence function                                      */
/* ------------------------------------------------------------ */
static bool sequence_initialized = false;
static void ensure_sequence(void)
{
    if (sequence_initialized) return;
    init_primes();
    terms = NULL;
    terms_cap = 0;
    terms_cnt = 0;
    next_candidate = 1;
    sequence_initialized = true;
}

seq_t a(idx_t n)
{
    ensure_sequence();

    while (terms_cnt < (size_t)n) {
        ++next_candidate;
        if (in_sequence(next_candidate)) {
            ensure_terms_cap(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)next_candidate;
        }
    }
    return terms[n - 1];   /* 1‑based index */
}
