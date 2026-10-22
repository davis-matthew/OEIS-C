/*
 * A383169  Triangle T(n,k) read by rows: For closed chains of j identical
 *          regular polygons with connecting inner vertices lying n vertices
 *          apart, the n-th row lists the possible j in descending order;
 *          n>=0, 1<=k<=d(8+4n).
 * https://oeis.org/A383169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (k‑th) divisor of 8+4*m in descending order, where m
 *            is the smallest nonnegative integer such that Σ_{i=0}^{m-1}
 *            d(8+4i) < n ≤ Σ_{i=0}^{m} d(8+4i) (d(x) = number of positive
 *            divisors of x). The returned value is 2 + that divisor. The
 *            sequence starts with 10, 6, 4, 3, 14, 8, 6, 5, 4, 3...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------
   Prime table (once initialised) for factorisation
   ------------------------------------------------------------ */
#define PRIME_TABLE_SIZE 1024

static uint64_t prime_table[PRIME_TABLE_SIZE];
static size_t prime_count = 0;

static void init_primes(void) {
    if (prime_count > 0) return;          /* already initialised */
    bool is_comp[PRIME_TABLE_SIZE];
    memset(is_comp, 0, sizeof(is_comp));
    for (int p = 2; p < (int)PRIME_TABLE_SIZE; ++p) {
        if (!is_comp[p]) {
            prime_table[prime_count++] = (uint64_t)p;
            if ((int64_t)p * p < (int)PRIME_TABLE_SIZE) {
                for (int q = p * p; q < (int)PRIME_TABLE_SIZE; q += p)
                    is_comp[q] = true;
            }
        }
    }
}

/* ------------------------------------------------------------
   Factorisation helpers
   ------------------------------------------------------------ */
static void factor_uint64(uint64_t n,
                          uint64_t *pf, int *exp, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_table[i] * prime_table[i] <= n; ++i) {
        uint64_t p = prime_table[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[(*cnt)++] = p;
            exp[(*cnt)-1] = e;
        }
    }
    if (n > 1) {           /* remaining prime factor */
        pf[(*cnt)++] = n;
        exp[(*cnt)-1] = 1;
    }
}

/* divisor count function */
static size_t divisor_count_uint64(uint64_t n)
{
    uint64_t pf[64];
    int exp[64];
    size_t cnt;
    factor_uint64(n, pf, exp, &cnt);
    size_t dcnt = 1;
    for (size_t i = 0; i < cnt; ++i) dcnt *= (size_t)(exp[i] + 1);
    return dcnt;
}

/* ------------------------------------------------------------
   Generate all divisors (ascending order)
   ------------------------------------------------------------ */
static void gen_divisors_rec(uint64_t mult, size_t idx, size_t cnt,
                             const uint64_t *pf, const int *exp,
                             uint64_t *divs, size_t *divc)
{
    if (idx == cnt) {
        divs[(*divc)++] = mult;
        return;
    }
    uint64_t p = pf[idx];
    uint64_t val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors_rec(mult * val, idx + 1, cnt, pf, exp, divs, divc);
        val *= p;
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* ------------------------------------------------------------
   The required sequence function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based, correspond to the flattened triangle */
    if (n == 0) return 0;   /* undefined, but safe */

    init_primes();

    idx_t cumulative = 0;
    for (idx_t row = 0;; ++row) {
        uint64_t m = 8 + 4ULL * (uint64_t)row;
        size_t rowlen = divisor_count_uint64(m);
        if (cumulative + (idx_t)rowlen >= n) {
            /* this is the row containing a(n) */
            idx_t k = n - cumulative;               /* 1‑based within row */
            /* factorisation */
            uint64_t pf[64];
            int exp[64];
            size_t pfcnt;
            factor_uint64(m, pf, exp, &pfcnt);
            /* generate all divisors */
            static uint64_t divs[1024];
            size_t divc = 0;
            gen_divisors_rec(1ULL, 0, pfcnt, pf, exp, divs, &divc);
            /* divc == rowlen should hold */
            qsort(divs, divc, sizeof(uint64_t), cmp_uint64);   /* ascending */
            size_t idx = divc - (size_t)k;   /* k‑th divisor in descending order */
            uint64_t divisor = divs[idx];
            return (seq_t)(2 + divisor);
        }
        cumulative += (idx_t)rowlen;
    }
}
