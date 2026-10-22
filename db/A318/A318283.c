/*
 * A318283  Sum of elements of the multiset spanning an initial interval of
 *          positive integers with multiplicities equal to the prime indices
 *          of n in weakly decreasing order.
 * https://oeis.org/A318283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{k-1} (i+1) * indices[i] where indices are the
 *            1‑based prime indices of the prime factors of n, repeated
 *            according to multiplicity, sorted in weakly decreasing order
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static tables holding primes up to the maximum n seen so far */
static idx_t   _prime_limit = 0;       /* largest n for which primes are known   */
static uint64_t * _prime_list = NULL;  /* array of primes                         */
static size_t    _prime_cnt   = 0;     /* number of primes                        */
static uint32_t * _prime_index = NULL; /* mapping prime→1‑based index (0 if not prime) */

/* ------------------------------------------------------------------ */
/*  init_primes_upto(limit)
    Builds a list of all primes ≤ limit and an array that maps each
    prime to its 1‑based index.  Data are reused for larger limits.     */
static void init_primes_upto(idx_t limit)
{
    if (limit <= _prime_limit) return;          /* already ready */

    /* free old data */
    free(_prime_list);
    free(_prime_index);

    /* Sieve of Eratosthenes – boolean array */
    char *is_prime = (char *)calloc((size_t)(limit + 1), sizeof(char));
    if (!is_prime) return;                      /* allocation failure */

    is_prime[0] = 1;  is_prime[1] = 1;           /* 0 and 1 are not prime */

    /* rough upper bound for number of primes: limit/ln(limit)+10 */
    size_t est = (size_t)(limit / 10 + 10);
    uint64_t *prime_tmp = (uint64_t *)malloc(est * sizeof(uint64_t));
    if (!prime_tmp) { free(is_prime); return; }

    _prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i]) {
            if (_prime_cnt == est) {             /* grow temporary array */
                est *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_tmp,
                                      est * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(prime_tmp); return; }
                prime_tmp = tmp;
            }
            prime_tmp[_prime_cnt++] = (uint64_t)i;

            if (i <= limit / i)                  /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = 1;
        }
    }

    /* allocate exact sized prime list */
    _prime_list = (uint64_t *)malloc(_prime_cnt * sizeof(uint64_t));
    if (!_prime_list) { free(is_prime); free(prime_tmp); return; }
    memcpy(_prime_list, prime_tmp, _prime_cnt * sizeof(uint64_t));

    /* mapping from prime value to its 1‑based index (0 if not prime) */
    _prime_index = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!_prime_index) { free(is_prime); free(prime_tmp); free(_prime_list); return; }
    for (size_t i = 0; i < _prime_cnt; ++i)
        _prime_index[(size_t)prime_tmp[i]] = (uint32_t)(i + 1);

    free(prime_tmp);
    free(is_prime);
    _prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  a(n)
 *  Computes nth term of A318283.  Works for any n≥1.                     */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* safety for offset */

    /* Ensure primes up to n are available */
    init_primes_upto(n);

    /* list of prime indices, maximum needed is small — fixed array */
    enum { MAX_FACTORS = 128 };
    uint32_t indices[MAX_FACTORS];
    size_t k = 0;

    uint64_t m = (uint64_t)n;
    for (size_t pi = 0; pi < _prime_cnt && (_prime_list[pi] * _prime_list[pi] <= m); ++pi) {
        uint64_t p = _prime_list[pi];
        while ((m % p) == 0) {
            if (k < MAX_FACTORS)
                indices[k++] = _prime_index[(size_t)p];
            m /= p;
        }
    }
    if (m > 1) {                     /* remaining prime factor */
        if (k < MAX_FACTORS)
            indices[k++] = _prime_index[(size_t)m];
    }

    if (k == 0) return (seq_t)0;     /* n == 1 */

    /* sort indices in non‑increasing order (insertion sort) */
    for (size_t i = 1; i < k; ++i) {
        uint32_t key = indices[i];
        size_t j = i;
        while (j > 0 && indices[j - 1] < key) {
            indices[j] = indices[j - 1];
            --j;
        }
        indices[j] = key;
    }

    /* compute sum Σ (i+1)*indices[i] */
    seq_t sum = 0;
    for (size_t i = 0; i < k; ++i)
        sum += (seq_t)(i + 1) * (seq_t)indices[i];

    return sum;
}
