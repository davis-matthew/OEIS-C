/*
 * A302194  Hurwitz inverse of [1 followed by primes], [1,2,3,5,7,...].
 * https://oeis.org/A302194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A302194 - Hurwitz inverse of [1 followed by primes].
 * EGf: 1 / ( 1 + Σ_{n≥1} prime(n)·x^n/n! )
 * Recurrence: a(0)=1
 * For m≥1:
 *   a(m) = - Σ_{k=0}^{m-1} C(m,k) · a(k) · prime(m-k)
 * where prime(j) is j‑th prime for j≥1 and prime(0)=1.
 *
 * The implementation uses this recurrence with incremental binomial
 * coefficients.  All arithmetic is performed with the seq_t type
 * (int8_t … __int128 or arbitrary‑precision integer).  A simple
 * sieve is used to generate the first few primes once.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are provided by the build environment. */
extern idx_t _floor_isqrt_u64(idx_t n);
extern size_t _prime_limit_for_idx(idx_t n);

/* Utility: allocate an array of seq_t of given size.  The caller
 * must free it with free(). */
static seq_t* alloc_seq_array(size_t n) {
    return (seq_t*)malloc(n * sizeof(seq_t));
}

/* ------------------------------------------------------------ */
/* Prime generation: first 11000 primes are enough for n up to 10000
   (the 10000th prime is 104729).  The sieve is performed once. */
static seq_t* primes_cache = NULL;
static size_t primes_cnt = 0;

static void init_primes(void) {
    if (primes_cnt > 0)
        return;
    const size_t LIMIT = 200000; /* covers >11000 primes */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    primes_cache = alloc_seq_array(11000);
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT && cnt < 11000; i++) {
        if (!is_comp[i]) {
            primes_cache[cnt++] = (seq_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_cnt = cnt;
}

/* ------------------------------------------------------------ */
/* Cache for computed sequence values.  The cache is enlarged
   only when necessary. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;   /* number of elements allocated */
static idx_t a_cache_max = 0;     /* highest index computed so far */

/* Ensure the cache can hold at least size elements. */
static void ensure_cache_size(size_t size) {
    if (a_cache_size >= size)
        return;
    a_cache = (seq_t*)realloc(a_cache, size * sizeof(seq_t));
    a_cache_size = size;
}

/* ------------------------------------------------------------ */
/* Main sequence routine */
seq_t a(idx_t n) {
    init_primes();                /* primes must be ready */

    /* Allocate cache large enough for n+1 elements. */
    ensure_cache_size((size_t)(n + 1));
    if (a_cache_max == 0 && (a_cache[0] = 1, true)) { /* initialize a(0) */
        a_cache_max = 0;
    }

    /* Compute values up to n if needed. */
    while (a_cache_max < n) {
        idx_t m = a_cache_max + 1;          /* next index to compute */

        seq_t sum = 0;      /* Σ C(m,k)·a(k)·prime(m-k) */
        seq_t binom = 1;    /* C(m,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* prime(m-k): if m-k==0 -> 1, else primes_cache[m-k-1] */
            seq_t pk = (m - k == 0) ?
                        (seq_t)1 :
                        primes_cache[(size_t)(m - k - 1)];
            seq_t term = a_cache[(size_t)k] * pk;
            term *= binom;          /* term = C(m,k) * a(k) * prime(m-k) */
            sum += term;

            /* Update binom for next k: C(m,k+1) = C(m,k)*(m-k)/(k+1) */
            if (k + 1 < m) {
                seq_t f1 = (seq_t)(m - k);
                seq_t f2 = (seq_t)(k + 1);
                binom = binom * f1 / f2;
            }
        }

        a_cache[(size_t)m] = -sum;   /* a(m) = -sum */
        a_cache_max = m;
    }

    return a_cache[(size_t)n];
}
