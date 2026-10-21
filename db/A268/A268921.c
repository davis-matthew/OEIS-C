/*
 * A268921  Irregular triangle read by rows: T(n, k) = Legendre symbol (-k /
 *          prime(n)) for k = 0, ..., prime(n)-1, for n >= 1, with T(n, 0) =
 *          0, T(1, 1) = 1.
 * https://oeis.org/A268921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the row‑by‑row traversal of the irregular triangle
 *            T(m,k)=Legendre(-k/p_m), where p_m is the m‑th prime. For the
 *            nth element (1‑based) we find the row m such that Σ_{i=1}^{m-1}
 *            p_i < n ≤ Σ_{i=1}^{m} p_i, let k = n - (Σ_{i=1}^{m-1} p_i + 1)
 *            (offset within the row), and a(n)=Legendre(p_m -...
 * Range:     i8 n <= 2587, i16 n <= 5117, i32 n <= 5117, i64 n <= 5117,
 *            i128 n <= 5117, big any n
 * Helpers:   is_prime_small, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime_small.h"

/* static storage for primes and cumulative sums */
static uint32_t *primes      = NULL;      /* sequence of primes */
static idx_t    *cum_sum     = NULL;      /* Σ primes[0..i] (1‑based rows) */
static size_t    prime_cnt   = 0;          /* number of primes stored */
static size_t    prime_cap   = 0;          /* allocated capacity */

/* helper: grow storage arrays to at least new_cap elements */
static void _grow_arrays(size_t new_cap)
{
    uint32_t *pnew = (uint32_t *)realloc(primes,   new_cap * sizeof(uint32_t));
    idx_t    *cnew = (idx_t   *)realloc(cum_sum,  new_cap * sizeof(idx_t));
    /* allocation failure ignored – program will abort naturally */
    if (pnew) primes = pnew;
    if (cnew) cum_sum = cnew;
    prime_cap = new_cap;
}

/* helper: extend prime list until the cumulative sum ≥ target_n */
static void _ensure_primes(idx_t target_n)
{
    if (prime_cnt == 0) {
        size_t init_cap = 16;
        primes = (uint32_t *)calloc(init_cap, sizeof(uint32_t));
        cum_sum = (idx_t *)calloc(init_cap, sizeof(idx_t));
        prime_cap = init_cap;
    }
    /* generate primes until we have enough cumulative sum */
    while (prime_cnt == 0 || cum_sum[prime_cnt-1] < target_n) {
        /* find next prime candidate */
        uint64_t cand = prime_cnt == 0 ? 2 : (uint64_t)primes[prime_cnt-1] + 1;
        while (!is_prime_small((seq_t)cand)) cand++;
        /* store new prime */
        if (prime_cnt == prime_cap) _grow_arrays(prime_cap * 2);
        primes[prime_cnt] = (uint32_t)cand;
        if (prime_cnt == 0)
            cum_sum[0] = cand;
        else
            cum_sum[prime_cnt] = cum_sum[prime_cnt-1] + cand;
        prime_cnt++;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but return 0 */
    _ensure_primes(n);

    /* locate the row r (0‑based index for arrays) */
    size_t r = 0;
    while (cum_sum[r] < n) r++;

    uint32_t p  = primes[r];                               /* prime of the row */
    idx_t    start = (r == 0 ? 1 : cum_sum[r-1] + 1);      /* first index of this row */
    idx_t    k = n - start;                                 /* 0‑based column in the row */

    /* Legendre symbol (-k/p) = Legendre((p-k)/p) */
    uint64_t aa = (uint64_t)p - (uint64_t)k;                /* p-k, guaranteed < p */
    int val = jacobi_symbol(aa, (uint64_t)p);               /* returns -1,0,1 */

    return (seq_t)val;
}
