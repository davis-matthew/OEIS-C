/*
 * A266909  Table read by rows: for each k < n and coprime to n, the least
 *          x>=0 such that x*n+k is prime.
 * https://oeis.org/A266909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5714, u16 n <= 10975, u32 n <= 10975, u64 n <= 10975,
 *            u128 n <= 10975, big any n
 * Validated: standard, pass, 119 terms through n = 9050 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A266909 – table read by rows:  for each n ≥ 2 and each
 * k < n coprime to n, return the smallest x ≥ 0 such that n·x + k is
 * prime.  The sequence is indexed starting at 1 for the first element
 * (the pair n = 2, k = 1).  The function maps an index to its
 * corresponding (n,k) pair via totient sums, then searches for the
 * desired x by trial. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Simple GCD for idx_t ------------------------------------------------ */
static inline bool coprime(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a == 1;
}

/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
/* Storage for φ values and their cumulative sums. ------------------- */
static idx_t *phi_arr   = NULL; /* φ[i] */
static idx_t *pref_arr  = NULL; /* Σ_{j=2}^{i} φ(j) */
static idx_t  max_phi   = 0;    /* highest i for which tables are valid */

/* --------------------------------------------------------------- */
/* Build φ[0..limit] and pref[0..limit]. ----------------------------- */
static void recompute_limit(idx_t limit)
{
    idx_t *new_phi  = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    idx_t *new_pref = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    if (!new_phi || !new_pref) exit(EXIT_FAILURE);

    for (idx_t i = 2; i <= limit; ++i) new_phi[i] = i;

    for (idx_t i = 2; i <= limit; ++i) {
        if (new_phi[i] == i) {                 /* i is prime */
            for (idx_t j = i; j <= limit; j += i)
                new_phi[j] -= new_phi[j] / i;
        }
    }

    new_pref[0] = new_pref[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        new_pref[i] = new_pref[i - 1] + new_phi[i];

    free(phi_arr);
    free(pref_arr);
    phi_arr   = new_phi;
    pref_arr  = new_pref;
    max_phi   = limit;
}

/* --------------------------------------------------------------- */
/* Ensure that φ and pref are computed up to an index with
 * cumulative totient sum ≥ 'need'. ----------------------------------- */
static void ensure_until(idx_t need)
{
    if (max_phi >= need) return;

    idx_t limit = max_phi < 2 ? 2 : max_phi;
    while (true) {
        if (limit < need) limit = need;
        recompute_limit(limit);
        if (pref_arr[limit] >= need) break;
        limit <<= 1;                      /* double until sufficient */
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function. ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined, guard */

    /* Build enough tables to locate the required row */
    ensure_until(n);

    /* Find the smallest row such that pref[row] ≥ n */
    idx_t row = 2;
    for (; row <= max_phi; ++row) {
        if (pref_arr[row] >= n) break;
    }

    idx_t prev_sum   = (row > 2) ? pref_arr[row - 1] : 0;
    idx_t pos_in_row = n - prev_sum - 1;     /* zero‑based index among coprimes */

    /* Locate the pos_in_row‑th coprime integer k (1 ≤ k < row) */
    idx_t k = 0;
    idx_t count = 0;
    for (k = 1; k < row; ++k) {
        if (coprime(row, k)) {
            if (count == pos_in_row) break;
            ++count;
        }
    }

    /* Search for the minimal x ≥ 0 with row·x + k prime */
    idx_t x = 0;
    for (;;) {
        seq_t candidate = (seq_t)row * (seq_t)x + (seq_t)k;
        if (is_prime_big(candidate)) break;
        ++x;
    }

    return (seq_t)x;
}
