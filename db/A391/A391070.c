/*
 * A391070  Irregular triangular array read by rows. T(n,k) is the number of
 *          integers in [n] that are multiples of the squarefree kernel of d
 *          but not divisible by a prime in the squarefree kernel of n/d where
 *          d is the k-th unitary divisor of n (as in A077610).
 * https://oeis.org/A391070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = read the triangular array T(n,k), where T(n,k) = # {x ∈
 *            [1..n] : rad(d) | x and gcd(x, rad(n/d)) = 1}, d is the k-th
 *            unitary divisor of n. The algorithm generates rows up to the
 *            index needed, keeps a cumulative count of terms, then computes
 *            T(row,k) via inclusion–exclusion
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Small helper: factor a positive integer using trial division. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t val = n;
    for (uint64_t p = 2; p * p <= val; ++p) {
        if (val % p == 0) {
            primes[*cnt] = p;
            exps[*cnt] = 0;
            while (val % p == 0) {
                val /= p;
                ++exps[*cnt];
            }
            ++(*cnt);
        }
    }
    if (val > 1) {
        primes[*cnt] = val;
        exps[*cnt] = 1;
        ++(*cnt);
    }
}

/* Structure holding a unitary divisor with its squared‑free kernel and mask. */
typedef struct {
    uint64_t d;      /* the divisor itself */
    uint64_t rad_d;  /* product of distinct primes in d */
    uint64_t mask;   /* bitmask indicating which primes are in d */
} unit_t;

/* Comparator for qsort: sort by divisor value. */
static int cmp_unit(const void *a, const void *b)
{
    const unit_t *ua = (const unit_t *)a;
    const unit_t *ub = (const unit_t *)b;
    if (ua->d < ub->d) return -1;
    if (ua->d > ub->d) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; undefined for 0 */

    /* --- Cached cumulative counts of terms per row --- */
    static idx_t *cum = NULL;         /* cum[i] = total terms up to row i+1 */
    static size_t cum_cap = 0;
    static size_t cum_len = 0;
    static idx_t cum_total = 0;       /* total terms up to the last row */

    /* Expand precomputed rows until we cover n. */
    uint64_t row = cum_len + 1;        /* next row number to process */
    while (cum_total < n) {
        /* factor the row number */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_uint64(row, primes, exps, &cnt);

        /* number of unitary divisors is 2^cnt */
        idx_t row_terms = 1ULL << cnt;
        cum_total += row_terms;

        /* enlarge cumulative array if needed */
        if (cum_len == cum_cap) {
            size_t new_cap = cum_cap ? cum_cap * 2 : 4;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) /* allocation failure should not happen in tests */
                return 0;
            cum = tmp;
            cum_cap = new_cap;
        }
        cum[cum_len++] = cum_total;

        ++row;
    }

    /* Find the row that contains position n by binary search. */
    size_t low = 0, high = cum_len - 1;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t row_num = low + 1;                     /* 1‑based row number */
    idx_t prev_cum = (low == 0) ? 0 : cum[low - 1];
    idx_t k = n - prev_cum;                         /* 1‑based within the row */

    /* Factor the chosen row again to build its unitary divisors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64(row_num, primes, exps, &cnt);

    /* Generate all unitary divisors. */
    size_t total_divs = 1ULL << cnt;
    unit_t *units = (unit_t *)malloc(total_divs * sizeof(unit_t));
    if (!units) return 0;  /* allocation failure, should not occur */

    for (size_t mask = 0; mask < total_divs; ++mask) {
        uint64_t d = 1;
        uint64_t rad_d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                /* multiply d by prime[i] ^ exps[i] */
                uint64_t pow = 1;
                for (size_t e = 0; e < exps[i]; ++e)
                    pow *= primes[i];
                d *= pow;
                rad_d *= primes[i];
            }
        }
        units[mask].d = d;
        units[mask].rad_d = rad_d;
        units[mask].mask = mask;
    }

    /* Sort divisors in ascending order of their value. */
    qsort(units, total_divs, sizeof(unit_t), cmp_unit);

    /* Retrieve the k‑th divisor and its associated data. */
    unit_t chosen = units[k - 1];
    free(units);   /* no longer needed */

    /* Build list of primes that are in n/d (i.e., not in the divisor). */
    uint64_t primes_nd[64];
    size_t primes_nd_cnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (!(chosen.mask & (1ULL << i)))
            primes_nd[primes_nd_cnt++] = primes[i];
    }

    /* Inclusion–exclusion to count numbers <= row_num divisible by rad_d
       and not divisible by any prime in primes_nd. */
    unsigned long long L = row_num / chosen.rad_d;   /* floor division */
    long long res = 0;        /* result fits in 64 bits for these limits */

    size_t subsets = 1ULL << primes_nd_cnt;
    for (size_t s = 0; s < subsets; ++s) {
        unsigned long long prod = 1;
        size_t bits = 0;
        for (size_t j = 0; j < primes_nd_cnt; ++j) {
            if (s & (1ULL << j)) {
                prod *= primes_nd[j];
                ++bits;
            }
        }
        unsigned long long term = L / prod;
        if (bits & 1)
            res -= term;
        else
            res += term;
    }

    return (seq_t)res;
}
