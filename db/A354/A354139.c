/*
 * A354139  a(n) is the least positive integer m such that (k+1)^n + (k+2)^n +
 *          ... + (k+m)^n == 0 (mod n) for every positive integer k.
 * https://oeis.org/A354139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that for every integer k, sum_{i=1}^m
 *            (k+i)^n ≡ 0 (mod n)
 * Range:     u8 n <= 39, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   swap_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm generates all divisors of n, tries each candidate
    m = rad(n) * d (rad = product of distinct prime factors of n),
    and tests the condition using the congruence
      sum_{i=1}^m (k+i)^n ≡ (blocks * S_total + partial) (mod n),
    where blocks = m / n and partial is a short partial sum that
    covers the remainder c = m % n.  The test is performed for all
    k = 0 … n-1, which is sufficient because the property must hold
    for all residues modulo n.  All arithmetic fits in idx_t
    (uint64_t) because the maximal m is bounded by n * rad(n)
    (≤ n² ≤ 10⁸ for n ≤ 10⁴). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/swap_ull.h"

/* fast modular power with indices of type idx_t */
static idx_t pow_mod_idx(idx_t base, idx_t exp, idx_t mod)
{
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* bubble‑sort small array of unsigned long long */
static void sort_divisors(unsigned long long *arr, size_t len)
{
    for (size_t i = 0; i < len - 1; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (arr[i] > arr[j]) {
                swap_ull(&arr[i], &arr[j]);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* special case -------------------------------------------------------- */
    if (n == 1) return (seq_t)1;

    /* 1. factorisation of n ---------------------------------------------- */
    unsigned long long N = n;   /* copy in native type for convenience */
    unsigned long long temp = N;
    unsigned long long rad = 1;          /* product of distinct primes */

    /* arrays for primes and exponents (at most 12 primes for 64‑bit n) */
    unsigned long long prime[12];
    unsigned long long expv[12];
    size_t pf_cnt = 0;

    for (unsigned long long p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned long long e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            prime[pf_cnt] = p;
            expv[pf_cnt] = e;
            rad *= p;
            pf_cnt++;
        }
    }
    if (temp > 1) {
        prime[pf_cnt] = temp;
        expv[pf_cnt] = 1;
        rad *= temp;
        pf_cnt++;
    }

    /* 2. generate all divisors of n --------------------------------------- */
    unsigned long long *divs = (unsigned long long *)malloc(sizeof(unsigned long long));
    divs[0] = 1;
    size_t div_len = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        unsigned long long p = prime[i];
        unsigned long long pp = 1;
        size_t old_len = div_len;
        for (unsigned long long e = 1; e <= expv[i]; ++e) {
            pp *= p;
            for (size_t j = 0; j < old_len; ++j) {
                divs = (unsigned long long *)realloc(divs,
                                                     (div_len + 1) * sizeof(unsigned long long));
                divs[div_len++] = divs[j] * pp;
            }
        }
    }

    /* sort divisors */
    sort_divisors(divs, div_len);

    /* 3. pre‑compute i^n mod n for i = 1 … n -------------------------------- */
    idx_t *pow_arr = (idx_t *)malloc((n + 1) * sizeof(idx_t)); /* 1‑based */
    for (idx_t i = 1; i <= n; ++i) {
        pow_arr[i] = pow_mod_idx((idx_t)i, (idx_t)n, n);
    }

    /* 4. build prefix sums on a doubled array for fast partial sums -------- */
    size_t pref_len = 2 * n + 1;
    idx_t *pref = (idx_t *)malloc(pref_len * sizeof(idx_t));
    pref[0] = 0;
    for (size_t i = 1; i <= 2 * n; ++i) {
        idx_t val = pow_arr[(i - 1) % n + 1];
        idx_t sum = pref[i - 1] + val;
        if (sum >= n) sum -= n;
        pref[i] = sum;
    }
    idx_t S_total = pref[n]; /* sum_{i=1}^n i^n mod n */

    /* 5. test candidates ----------------------------------------------------- */
    for (size_t idx = 0; idx < div_len; ++idx) {
        unsigned long long d = divs[idx];
        unsigned long long m_ulong = rad * d;          /* candidate m    */
        unsigned long long blocks = m_ulong / N;       /* number of full blocks */
        idx_t c = m_ulong % N;                        /* remaining length  */

        idx_t blocks_mod = (blocks % N);              /* we only need modulo n   */

        bool ok = true;
        for (idx_t k = 0; k < N; ++k) {
            /* partial sum over c terms starting from position k+1 */
            idx_t start = k;
            idx_t partial = pref[start + c];
            partial = (partial + N - pref[start]) % N;

            idx_t total = (blocks_mod * S_total) % N;
            total = (total + partial) % N;
            if (total != 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            free(divs);
            free(pow_arr);
            free(pref);
            return (seq_t)m_ulong;
        }
    }

    /* theoretically we should always find a candidate */
    free(divs);
    free(pow_arr);
    free(pref);
    return (seq_t)0;  /* fallback, should never reach here */
}
