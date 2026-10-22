/*
 * A335127  A multiset whose multiplicities are the prime indices of n is
 *          separable.
 * https://oeis.org/A335127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer whose prime‑index multiset is
 *            separable (i.e., there exists a permutation of the multiset with
 *            no adjacent equal elements)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  ----------------------------------------------------------------
 *  Utilities used in this implementation
 *  ---------------------------------------------------------------- */

static const idx_t LIMIT = 300000;     /* sieve limit – enough for N≤10⁴ */

static uint32_t *spf = NULL;          /* smallest prime factor table   */
static uint32_t *primeIdx = NULL;     /* prime → its index (1‑based)   */

static size_t nPrimes = 0;            /* count of primes found         */
static uint32_t *primes = NULL;       /* array of primes               */

/*  Build linear sieve up to LIMIT, filling spf and primeIdx      */
static void build_sieve(void)
{
    spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    primeIdx = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    primes = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primeIdx[i] = (uint32_t)(++nPrimes);
            primes[nPrimes - 1] = (uint32_t)i;
        }
        for (size_t j = 0; j < nPrimes; ++j) {
            uint32_t p = primes[j];
            idx_t v = i * (idx_t)p;
            if (v > LIMIT) break;
            spf[v] = p;
            if (i % p == 0) break;
        }
    }
    free(primes);   /* primes array no longer needed after sieve */
}

/*  ----------------------------------------------------------------
 *  Core routine: test whether integer n is separable
 *  ---------------------------------------------------------------- */
static bool is_separable(idx_t n)
{
    if (n == 1) return true;    /* empty multiset – trivially separable */

    idx_t tmp = n;
    idx_t sum = 0;
    idx_t maxIdx = 0;

    while (tmp > 1) {
        uint32_t p = spf[tmp];
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        uint32_t idx = primeIdx[p];
        sum += idx * e;
        if (idx > maxIdx) maxIdx = idx;
    }

    /* Condition: maxIdx <= ceil(sum/2) == (sum+1)/2  */
    return maxIdx <= (sum + 1) / 2;
}

/*  ----------------------------------------------------------------
 *  Storage for pre‑computed separable numbers
 *  ---------------------------------------------------------------- */
static seq_t *sep = NULL;      /* array of separable numbers (values a(n))    */
static size_t sep_cap = 0;     /* allocated capacity                       */
static size_t sep_count = 0;   /* number of stored separable numbers         */
static idx_t max_checked = 0;  /* last integer checked                      */

static void ensure_sep_capacity(size_t needed)
{
    if (needed <= sep_cap) return;
    size_t new_cap = (sep_cap == 0) ? 16 : sep_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(sep, new_cap * sizeof(seq_t));
    sep = new_arr;
    sep_cap = new_cap;
}

static void compute_until(idx_t n)
{
    /* Build sieve only once */
    static bool sieve_built = false;
    if (!sieve_built) {
        build_sieve();
        sieve_built = true;
    }

    while (sep_count < n) {
        ++max_checked;
        if (is_separable(max_checked)) {
            ensure_sep_capacity(sep_count + 1);
            sep[sep_count++] = (seq_t)max_checked;
        }
    }
}

/*  ----------------------------------------------------------------
 *  Public function required by the problem
 *  ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, return 0 for safety */
    compute_until(n);
    return sep[n - 1];              /* array is 0‑based, a(1) at index 0 */
}
