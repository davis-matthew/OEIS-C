/*
 * A370821  Number of minimal deterministic Mealy automata with n states
 *          outputting ternary strings.
 * https://oeis.org/A370821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = psi(3, n) + Sum_{i=1..n-1} (3-1)*3^(i-1)*psi(3, n-i)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"

/* --- Utility: integer square root for small idx_t values --- */
static inline idx_t isqrt_idx(idx_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* --- Cached arrays --- */
static seq_t *psi_arr   = NULL;
static seq_t *pow3_arr  = NULL;
static seq_t *a_arr     = NULL;
static seq_t *s_arr     = NULL;          /* intermediate S(n) = sum_{i=1}^{n} 3^{i-1}*psi(n+1-i) */
static idx_t  cur_size  = 0;              /* largest index that has been computed (max n) */

static signed char *mu_arr  = NULL;       /* Möbius function values */
static idx_t  mu_max   = 0;               /* largest n for which mu_arr is valid */

/* --- Helper prototypes --- */
static void ensure_size(idx_t n);
static void ensure_mu(idx_t limit);
static seq_t compute_psi(idx_t n);

/* --- Public function --- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_size(n);
    return a_arr[n];
}

/* --- Private helpers --- */

/* Ensure that arrays are allocated up to at least n and that
   all values psi, a, and S are computed for indices 1..n. */
static void ensure_size(idx_t n)
{
    if (n <= cur_size) return;

    /* Determine new size (next power of two >= n+1) */
    idx_t new_cap = cur_size ? cur_size << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;

    /* Reallocate storage */
    psi_arr = (seq_t *)realloc(psi_arr, new_cap * sizeof(seq_t));
    pow3_arr = (seq_t *)realloc(pow3_arr, new_cap * sizeof(seq_t));
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    s_arr = (seq_t *)realloc(s_arr, new_cap * sizeof(seq_t));
    if (!psi_arr || !pow3_arr || !a_arr || !s_arr) exit(EXIT_FAILURE);

    /* Initialize new elements */
    for (idx_t i = cur_size + 1; i < new_cap; ++i) {
        psi_arr[i] = 0;
        pow3_arr[i] = 0;
        a_arr[i] = 0;
        s_arr[i] = 0;
    }

    /* Set base values */
    if (cur_size == 0) {
        pow3_arr[0] = (seq_t)1;   /* 3^0 = 1 */
        s_arr[0]   = (seq_t)0;    /* S(0) = 0 */
    }

    /* Compute powers of 3 for new indices */
    for (idx_t i = cur_size + 1; i < new_cap; ++i)
        pow3_arr[i] = seq_mul(pow3_arr[i - 1], (seq_t)3);

    /* Ensure Möbius function values are available up to new_cap-1 */
    ensure_mu(new_cap - 1);

    /* Compute psi, S, and a for new indices in increasing order */
    for (idx_t i = cur_size + 1; i < new_cap; ++i) {
        /* psi(i) via Möbius inversion */
        psi_arr[i] = compute_psi(i);

        /* S(i) = 3 * S(i-1) + psi(i) */
        seq_t tmp = seq_mul((seq_t)3, s_arr[i - 1]);
        s_arr[i]  = tmp + psi_arr[i];

        /* a(i) = psi(i) + 2 * S(i-1) */
        seq_t tmp2 = seq_mul((seq_t)2, s_arr[i - 1]);
        a_arr[i]   = psi_arr[i] + tmp2;
    }

    cur_size = new_cap - 1;
}

/* Compute Möbius function values up to 'limit' (inclusive) using a linear sieve. */
static void ensure_mu(idx_t limit)
{
    if (limit <= mu_max) return;

    /* Allocate storage for primes and Möbius values */
    signed char *new_mu = (signed char *)realloc(mu_arr, (limit + 1) * sizeof(signed char));
    if (!new_mu) exit(EXIT_FAILURE);
    mu_arr = new_mu;

    /* Simple sieve to compute mu recursively */
    mu_arr[0] = 0;
    mu_arr[1] = 1;

    /* Temporary array to mark composites */
    char *is_comp = (char *)calloc(limit + 1, sizeof(char));
    if (!is_comp) exit(EXIT_FAILURE);

    idx_t *primes = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            mu_arr[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > limit) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu_arr[ip] = 0;
                break;
            } else {
                mu_arr[ip] = -mu_arr[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    mu_max = limit;
}

/* Compute psi(3, n) = Σ_{d|n} μ(d) * 3^{n/d} */
static seq_t compute_psi(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t root = isqrt_idx(n);
    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            signed char mu1 = mu_arr[d];
            if (mu1 == 1)       sum += pow3_arr[d2];
            else if (mu1 == -1) sum -= pow3_arr[d2];

            if (d2 != d) {
                signed char mu2 = mu_arr[d2];
                if (mu2 == 1)       sum += pow3_arr[d];
                else if (mu2 == -1) sum -= pow3_arr[d];
            }
        }
    }
    return sum;
}
