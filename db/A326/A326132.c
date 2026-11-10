/*
 * A326132  Numbers n for which A294898(n) is not zero and A294898(n) divides
 *          A000120(n); numbers for which A326130(n) = abs(A294898(n)).
 * https://oeis.org/A326132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th number k with d(k) =
 *            2k - sigma(k), d(k) - A000120(k) != 0 and |d(k) - A000120(k)|
 *            dividing A000120(k). That forces 0 <= d(k) <= 2*A000120(k) <=
 *            128. Candidates k <= X are listed by a depth-first search over
 *            prime factorizations (primes in increasing or...
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 52 (2026-11-08)
 * Helpers:   is_prime_u64, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/popcount_u64.h"

#define A326132_PLIM (1u << 21)
#define A326132_DMAX 128

static uint32_t *A326132_pr = NULL;
static size_t A326132_npr = 0;
static uint64_t A326132_X = 0;
static uint64_t *A326132_terms = NULL;
static size_t A326132_cnt = 0, A326132_cap = 0;

static void A326132_sieve(void)
{
    unsigned char *c = (unsigned char *)malloc(A326132_PLIM + 1);
    uint32_t i, j;
    memset(c, 0, A326132_PLIM + 1);
    A326132_pr = (uint32_t *)malloc(sizeof(uint32_t) * 160000);
    for (i = 2; i <= A326132_PLIM; i++) {
        if (c[i]) continue;
        A326132_pr[A326132_npr++] = i;
        if ((uint64_t)i * i <= A326132_PLIM)
            for (j = i * i; j <= A326132_PLIM; j += i) c[j] = 1;
    }
    free(c);
}

static void A326132_adv(size_t *i, uint64_t *q)
{
    if (*i + 1 < A326132_npr) {
        (*i)++;
        *q = A326132_pr[*i];
    } else {
        uint64_t x = *q + 1;
        *i = A326132_npr;
        while (!is_prime_u64(x)) x++;
        *q = x;
    }
}

static void A326132_add(uint64_t n)
{
    if (A326132_cnt == A326132_cap) {
        A326132_cap = A326132_cap ? 2 * A326132_cap : 256;
        A326132_terms = (uint64_t *)realloc(A326132_terms, sizeof(uint64_t) * A326132_cap);
    }
    A326132_terms[A326132_cnt++] = n;
}

static void A326132_chk(uint64_t n, unsigned __int128 sig)
{
    unsigned __int128 two = (unsigned __int128)n * 2;
    long long d, pop, x;
    if (sig > two) return;
    if (two - sig > (unsigned __int128)A326132_DMAX) return;
    d = (long long)(two - sig);
    pop = (long long)popcount_u64(n);
    x = d - pop;
    if (x == 0) return;
    if (x < 0) x = -x;
    if (pop % x == 0) A326132_add(n);
}

static void A326132_dfs(uint64_t m, uint64_t s, size_t ci, uint64_t cq)
{
    size_t i = ci;
    uint64_t q = cq;
    uint64_t X = A326132_X;
    for (;;) {
        double need, B;
        uint64_t rem, prod, pj, mm, pe, sp;
        size_t j;
        if ((unsigned __int128)m * q * q > X) break;
        need = 2.0 - (double)A326132_DMAX / ((double)m * (double)q * (double)q);
        B = (double)s / (double)m;
        rem = X / m;
        prod = 1;
        j = i;
        pj = q;
        while ((unsigned __int128)prod * pj <= rem) {
            prod *= pj;
            B *= (double)pj / (double)(pj - 1);
            A326132_adv(&j, &pj);
        }
        if (B * (1.0 + 1e-9) < need) break;
        mm = m; pe = 1; sp = 1;
        while ((unsigned __int128)mm * q <= X) {
            unsigned __int128 ss;
            mm *= q;
            pe *= q;
            sp += pe;
            ss = (unsigned __int128)s * sp;
            A326132_chk(mm, ss);
            if (ss < (unsigned __int128)mm * 2) {
                size_t ni = i;
                uint64_t nq = q;
                A326132_adv(&ni, &nq);
                A326132_dfs(mm, (uint64_t)ss, ni, nq);
            }
        }
        A326132_adv(&i, &q);
    }
    /* last prime q (exponent 1), q >= first prime not visited above */
    {
        uint64_t D = 2 * m - s;
        uint64_t lo = (s + D - 1) / D;
        uint64_t hi = (s + A326132_DMAX) / D;
        uint64_t qq;
        if (lo < q) lo = q;
        for (qq = lo; qq <= hi; qq++) {
            if ((unsigned __int128)m * qq > X) break;
            if (is_prime_u64(qq))
                A326132_chk(m * qq, (unsigned __int128)s * (qq + 1));
        }
    }
}

static int A326132_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A326132_npr == 0) A326132_sieve();
    while (A326132_X == 0 || A326132_cnt < n) {
        size_t k, w;
        if (A326132_X == 0) A326132_X = (uint64_t)1 << 16;
        else if (A326132_X > ((uint64_t)1 << 58)) return 0;
        else A326132_X <<= 3;
        A326132_cnt = 0;
        A326132_dfs(1, 1, 0, 2);
        qsort(A326132_terms, A326132_cnt, sizeof(uint64_t), A326132_cmp);
        w = 0;
        for (k = 0; k < A326132_cnt; k++)
            if (w == 0 || A326132_terms[w - 1] != A326132_terms[k])
                A326132_terms[w++] = A326132_terms[k];
        A326132_cnt = w;
    }
    return (seq_t)A326132_terms[n - 1];
}
