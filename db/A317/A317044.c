/*
 * A317044  Numbers k such that A(k+1) = A(k) + 1, where A() = A005100() are
 *          the deficient numbers.
 * https://oeis.org/A317044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = indices k such that A005100(k+1) = A005100(k)+1 (i.e., k
 *            and k+1 are consecutive deficient numbers). This implementation
 *            builds the sequence by testing integers for deficiency and
 *            memoising the indices found
 * Range:     u8 n <= 52, u16 n <= 1661, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- static cache for sequence terms ---------- */
static size_t a_len  = 0, a_cap = 0;
static seq_t *a_arr = NULL;

/* ---------- variables to generate deficient numbers ---------- */
static uint64_t cur_x             = 1;   /* next integer to test          */
static uint64_t def_index         = 0;   /* count of deficient numbers found */
static uint64_t last_def_val      = 0;   /* value of previous deficient number   */
static uint64_t last_def_index    = 0;   /* index (def_index) of the last deficient number   */

/* ---------- spf sieve storage ---------- */
static uint32_t *spf = NULL;        /* smallest prime factor of i        */
static size_t  spf_size = 0;        /* number of elements in spf (max i) */

static uint32_t *primes = NULL;     /* list of primes */
static size_t primes_cap = 0;       /* capacity of primes array */
static size_t primes_size = 0;      /* current number of primes */

/* ---------- helper: grow spf table to at least new_limit ---------- */
static void recompute_spf(size_t new_limit)
{
    /* Recreate spf and primes from scratch up to new_limit. */
    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    /* Allocate or expand primes array with a safe over‑estimate. */
    size_t prime_cap_est = new_limit + 10;   /* enough for all primes up to new_limit */
    if (prime_cap_est > primes_cap) {
        uint32_t *tmp = (uint32_t *)realloc(primes,
                                            prime_cap_est * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = prime_cap_est;
    }
    primes_size = 0;

    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {              /* i is prime */
            new_spf[i] = (uint32_t)i;
            primes[primes_size++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_size; ++j) {
            uint32_t p = primes[j];
            uint64_t mult = (uint64_t)i * p;
            if (mult > new_limit) break;
            new_spf[mult] = p;
            if (i % p == 0) break;
        }
    }

    free(spf);
    spf = new_spf;
    spf_size = new_limit;
}

/* ---------- helper: ensure spf table exists for all numbers <= n ---------- */
static void ensure_spf_for(size_t n)
{
    if (n <= spf_size) return;
    size_t new_limit = spf_size ? spf_size * 2 : 256;
    if (new_limit < n) new_limit = n * 2;
    recompute_spf(new_limit);
}

/* ---------- compute sigma(n) using spf table ---------- */
static seq_t sigma_of(uint64_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    uint64_t m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        /* term = (p^(exp+1) - 1) / (p - 1) */
        seq_t pow = (seq_t)1;
        for (int i = 0; i <= exp; ++i) pow *= (seq_t)p;
        seq_t term = (pow - (seq_t)1) / (seq_t)(p - 1);
        result *= term;
    }
    return result;
}

/* ---------- test whether n is deficient (sigma(n) < 2*n) ---------- */
static bool is_deficient(uint64_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t two_n = (seq_t)(2ULL * n);
    return sigma < two_n;
}

/* ---------- append a value to the cached sequence ---------- */
static void append_a_val(seq_t val)
{
    if (a_len == a_cap) {
        a_cap = a_cap ? a_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_arr = tmp;
    }
    a_arr[a_len++] = val;
}

/* ---------- public function: a(n) ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* if already computed, return cached value */
    if ((idx_t)a_len >= n) return a_arr[n - 1];

    /* generate terms until we have at least n entries stored */
    while (a_len < (size_t)n) {
        ensure_spf_for(cur_x);
        if (is_deficient(cur_x)) {
            ++def_index;
            if (last_def_val != 0 && cur_x - last_def_val == 1) {
                /* cur_x succeeds last_def_val; store index of the first one */
                append_a_val((seq_t)last_def_index);
            }
            last_def_val   = cur_x;
            last_def_index = def_index;
        }
        ++cur_x;
    }
    return a_arr[n - 1];
}
