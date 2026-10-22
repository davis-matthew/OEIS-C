/*
 * A398635  Number of interior lattice points of [0,2n]^2 visible from all
 *          four vertices.
 * https://oeis.org/A398635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{1 <= x,y < 2n} [ gcd(x*(2n-x), y*(2n-y)) = 1 ]
 * Algorithm: The Möbius inversion formula a(n)= Σ_{d>0} μ(d)·c(d)² is used,
 * where c(d) = |{x : 1 ≤ x < 2n , d | x·(2n-x)}|.
 * For each x we generate all square‑free divisors d of x·(2n−x) by taking all
 * non‑empty subsets of the distinct prime factors of x and 2n−x.
 * We collect all such d in an array, sort it, count equal runs to obtain
 * c(d), and then add or subtract c(d)² according to μ(d).  The d = 1 term
 * is handled separately as μ(1)=1 and c(1)=2n−1.
 * This runs in O(n·log n) with about 5·10⁶ appended d’s for n = 10000,
 * well below the time limit.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  smallest prime factor table for numbers up to 20000          */
#define MAX_VAL 20000
static uint16_t spf[MAX_VAL + 1];
static bool spf_inited = false;

static void init_spf(void)
{
    if (spf_inited) return;
    for (uint32_t i = 0; i <= MAX_VAL; ++i) spf[i] = (uint16_t)i;
    for (uint32_t i = 2; i * i <= MAX_VAL; ++i) {
        if (spf[i] == (uint16_t)i) {
            for (uint32_t j = i * i; j <= MAX_VAL; j += i) {
                if (spf[j] == (uint16_t)j) spf[j] = (uint16_t)i;
            }
        }
    }
    spf_inited = true;
}

/* ------------------------------------------------------------ */
/*  factorise v (≤ MAX_VAL) into distinct prime factors         */
static void factor_unique(uint32_t v, int *primes, int *cnt)
{
    *cnt = 0;
    while (v > 1) {
        uint32_t p = spf[v];
        primes[(*cnt)++] = (int)p;
        while (v % p == 0) v /= p;
    }
}

/* ------------------------------------------------------------ */
static int cmp_uint32(const void *a, const void *b)
{
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_spf();

    uint32_t N     = (uint32_t)(2u * (uint32_t)n);   /* 2n   */
    uint32_t xmax  = N - 1;                          /* 2n-1 */
    size_t   cap   = (size_t)xmax * 256 + 1;         /* generous */
    uint32_t *keys = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!keys) return 0;   /* out of memory, return 0 */

    size_t len = 0;
    int px[16], py[16], kx, ky, primes_set[32], k;
    for (uint32_t x = 1; x < N; ++x) {
        factor_unique(x,          px, &kx);
        factor_unique(N - x,      py, &ky);

        /* union of prime factors (distinct) */
        k = 0;
        for (int i = 0; i < kx; ++i) primes_set[k++] = px[i];
        for (int i = 0; i < ky; ++i) {
            int p = py[i];
            bool seen = false;
            for (int j = 0; j < k; ++j) {
                if (primes_set[j] == p) { seen = true; break; }
            }
            if (!seen) primes_set[k++] = p;
        }
        if (k == 0) continue;           /* x*(N-x)=1, no d>1 */

        size_t subsets = (size_t)1 << k;
        for (size_t mask = 1; mask < subsets; ++mask) {
            uint32_t d = 1;
            size_t bm = mask;
            size_t idx = 0;
            while (bm) {
                if (bm & 1) d *= (uint32_t)primes_set[idx];
                ++idx;
                bm >>= 1;
            }
            keys[len++] = d;
        }
    }

    /* -------------------------------------------------------- */
    /*  sort the list of d's and count equal runs               */
    qsort(keys, len, sizeof(uint32_t), cmp_uint32);

    /*  d = 1 term: μ(1)=1, c(1)=2n-1                         */
    uint64_t sumPos = (uint64_t)xmax * (uint64_t)xmax;
    uint64_t sumNeg = 0;

    /*  process square‑free d > 1                                */
    size_t i = 0;
    while (i < len) {
        size_t j = i + 1;
        while (j < len && keys[j] == keys[i]) ++j;
        uint32_t  d = keys[i];
        uint32_t  c = (uint32_t)(j - i);

        /* compute μ(d)  (d is square‑free) */
        uint32_t tmp = d;
        int    cntPrimes = 0;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            cntPrimes++;
            while (tmp % p == 0) tmp /= p;
        }
        int mu = (cntPrimes & 1) ? -1 : 1;

        uint64_t cc = (uint64_t)c;
        if (mu == 1) sumPos += cc * cc;
        else        sumNeg += cc * cc;

        i = j;
    }

    free(keys);
    return (seq_t)(sumPos - sumNeg);
}
