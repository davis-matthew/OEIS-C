/*
 * A362937  a(n) = (A000982(n) - A362931(n))/2.
 * https://oeis.org/A362937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000982(n) - A362931(n))/2
 * Range:     u8 n <= 22, u16 n <= 515, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A000982(n) = number of pairs (i,j) in [1..n]^2 with integer
 *  arithmetic mean.  This equals e^2 + o^2 where 
 *    e = floor(n/2), o = ceil(n/2).
 *
 *  A362931(n) = number of pairs (i,j) with integer harmonic mean.
 *  For each pair, write i=g*a, j=g*b with gcd(a,b)=1.
 *  The harmonic mean is integer iff a+b divides 2*g.
 *  Counting all ordered pairs reduces to:
 *
 *     sum_{g=1..n}   sum_{d | 2g}   #{ m in [max(1,d-l),min(l,d-1)]
 *                                         : gcd(m,d)=1 }     (1)
 *
 *  where l = floor(n/g).  (1) is evaluated by iterating over each
 *  divisor d of 2g and counting integers coprime to d in an interval.
 *  Coprime counts are obtained by inclusion–exclusion over the
 *  distinct prime factors of d.
 *
 *  The routine below precomputes divisor lists, smallest prime
 *  factors and distinct prime factors up to 2*max_n (dynamic
 *  on first call) and uses them to evaluate A362931(n).
 */

#include <stddef.h>    /* for size_t */
#include <stdint.h>    /* for uint8_t */

/* ---------------------------------------------------------------------- */
/*  Static data (dynamic allocation, expanded on first/next call)        */
/* ---------------------------------------------------------------------- */
#define DIV_PER_NUM 256   /* capacity per divisor list (sufficient for d<=20000) */
#define FACT_PER_NUM 5    /* capacity per distinct prime factor list          */

static size_t   prepared_max = 0;   /* maximum (2*n) prepared so far */

static int   *divCount  = NULL;          /* number of divisors for each d      */
static int  *divListData;                /* flattened divisors[d][k]          */
static uint8_t *factCount = NULL;        /* number of distinct prime factors  */
static int  *factListData;               /* flattened factors[d][k]           */
static int   *spf       = NULL;          /* smallest prime factor for each d  */

/* ---------------------------------------------------------------------- */
/*  Helper: inclusive–exclusive coprime count: #{ x in [1..X] : gcd(x,d)=1 } */
/* ---------------------------------------------------------------------- */
static inline int coprime_leq(int d, int X)
{
    if (X <= 0) return 0;
    int k = factCount[d];
    int64_t res = X;
    int mask_limit = 1 << k;
    for (int mask = 1; mask < mask_limit; ++mask) {
        int64_t prod = 1;
        int bits = 0;
        for (int i = 0; i < k; ++i) {
            if (mask & (1 << i)) {
                prod *= factListData[d * FACT_PER_NUM + i];
                ++bits;
            }
        }
        int64_t add = X / prod;
        if (bits & 1) res -= add; else res += add;
    }
    return (int)res;
}

/* ---------------------------------------------------------------------- */
/*  Prepare data structures up to 2*max_n (dynamic reallocation)        */
/* ---------------------------------------------------------------------- */
static void prepare(idx_t n)
{
    idx_t needed = n * 2;
    if (needed <= prepared_max) return;

    /* free old data if any */
    free(divCount);
    free(divListData);
    free(factCount);
    free(factListData);
    free(spf);

    size_t limit = needed + 1;  /* inclusive */

    /* allocate arrays */
    divCount     = (int*)     malloc(limit * sizeof(int));
    divListData  = (int*)     malloc(limit * DIV_PER_NUM * sizeof(int));
    factCount    = (uint8_t*) malloc(limit * sizeof(uint8_t));
    factListData = (int*)     malloc(limit * FACT_PER_NUM * sizeof(int));
    spf          = (int*)     malloc(limit * sizeof(int));

    if (!divCount || !divListData || !factCount || !factListData || !spf)
        return;   /* out of memory, but sequence callers will not crash */

    /* initialise divisor counts */
    for (size_t i = 0; i < limit; ++i) divCount[i] = 0;

    /* compute divisor lists */
    for (int d = 1; d < (int)limit; ++d) {
        for (int t = d; t < (int)limit; t += d) {
            int idx = t * DIV_PER_NUM + divCount[t];
            divListData[idx] = d;
            divCount[t]++;
        }
    }

    /* sieve for smallest prime factor */
    for (int i = 0; i < (int)limit; ++i) spf[i] = 0;
    spf[1] = 1;
    for (int i = 2; i < (int)limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((int64_t)i * i < (int64_t)limit) {
                for (int j = i * i; j < (int)limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* compute distinct prime factor lists */
    for (int d = 2; d < (int)limit; ++d) {
        int val = d;
        int fc = 0;
        int last = 0;
        while (val > 1) {
            int p = spf[val];
            if (p != last) {
                factListData[d * FACT_PER_NUM + fc++] = p;
                last = p;
            }
            val /= p;
        }
        factCount[d] = (uint8_t)fc;
    }
    factCount[1] = 0;  /* for d=1 */
    prepared_max = needed;
}

/* ---------------------------------------------------------------------- */
/*  Public function implementing A362937                                   */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; 0 is undefined but harmless */

    prepare(n);            /* ensure precomputed data up to 2*n */

    /* 1. Compute number of pairs with integer arithmetic mean */
    seq_t e = (seq_t)(n / 2);   /* even numbers */
    seq_t o = (seq_t)n - e;    /* odd numbers  */
    seq_t Arith = e * e + o * o;   /* e^2 + o^2 */

    /* 2. Compute number of pairs with integer harmonic mean */
    seq_t HMtotal = 0;

    for (idx_t g = 1; g <= n; ++g) {
        idx_t limit_g = n / g;          /* floor(n/g)  */
        int   divCnt  = divCount[2 * g];
        int   divBase = 2 * g * DIV_PER_NUM;

        for (int idx = 0; idx < divCnt; ++idx) {
            int d = divListData[divBase + idx];
            int lim = (int)limit_g;
            int L = d > lim ? d - lim : 1;
            if (L < 1) L = 1;
            int R = (d - 1 < lim) ? d - 1 : lim;
            if (L > R) continue;
            int cntR = coprime_leq(d, R);
            int cntL = coprime_leq(d, L - 1);
            int cnt = cntR - cntL;
            HMtotal += (seq_t)cnt;
        }
    }

    seq_t result = (Arith - HMtotal) / 2;
    return result;
}
