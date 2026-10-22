/*
 * A376570  Table T(n, k), n > 1, k = 1..n-1, read by rows; T(n, k) is the
 *          least m such that the points (m, prime(m)), (k, prime(k)) and (n,
 *          prime(n)) are aligned (where prime(k) denotes the k-th prime
 *          number).
 * https://oeis.org/A376570
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the element T(n,k) of triangle T, where T(n,k) is the
 *            least m (1 ≤ m ≤ k) such that the points (m,p(m)), (k,p(k)),
 *            (n,p(n)) are collinear. The sequence is read row‑wise starting
 *            with n = 2
 * Range:     u8 n <= 9271, u16 n <= 9271, u32 n <= 9271, u64 n <= 9271,
 *            u128 n <= 9271, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   Prime generation (1‑based indices).
   -------------------------------------------------------------------- */
static uint64_t *prime_arr = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;

/* Ensure prime_arr contains at least idx elements (1‑based). */
static void ensure_primes_up_to_idx(size_t idx)
{
    if (prime_cnt >= idx) return;               /* already enough */

    if (!prime_arr) {                           /* first allocation */
        prime_cap = 64;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate = (prime_cnt == 0) ? 2ULL : prime_arr[prime_cnt - 1] + 1ULL;
    while (prime_cnt < idx) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt &&
                 prime_arr[i] * prime_arr[i] <= candidate; ++i) {
            if (candidate % prime_arr[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {        /* grow array */
                prime_cap *= 2;
                prime_arr = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* --------------------------------------------------------------------
   Triangle entry T(n,k).
   -------------------------------------------------------------------- */
static seq_t T(idx_t n, idx_t k)
{
    /* Need primes up to indices n and k. */
    ensure_primes_up_to_idx(n);
    ensure_primes_up_to_idx(k);

    uint64_t pn = prime_arr[n - 1];
    uint64_t pk = prime_arr[k - 1];

    for (idx_t m = 1; m <= k; ++m) {
        uint64_t pm = prime_arr[m - 1];

        /* Collinearity test: (pk-pm)*(n-m) == (pn-pm)*(k-m) */
        uint64_t left  = (pk - pm) * (n - m);
        uint64_t right = (pn - pm) * (k - m);
        if (left == right) return (seq_t)m;
    }
    /* Fallback – theoretically unreachable (m = k always works). */
    return (seq_t)k;
}

/* --------------------------------------------------------------------
   Sequence accessor.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                       /* below offset */

    /* Find the row r (r ≥ 2) such that (r-1)*r/2 ≥ n-1. */
    idx_t r = 2;
    while (((r - 1) * r) / 2 < n - 1) ++r;

    /* Elements before row r: (r-2)*(r-1)/2. */
    idx_t prev = ((r - 2) * (r - 1)) / 2;

    /* Column index k (1 ≤ k ≤ r-1). */
    idx_t k = n - prev - 1;

    return T(r, k);
}
