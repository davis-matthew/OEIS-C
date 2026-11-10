/*
 * A383548  Record values in A383546.
 * https://oeis.org/A383548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A383546(A383547(n)), the record values of A383546.
 *            Search: A383546(m) = min over primes p not dividing m of (m^{-1}
 *            mod p^2) - 1, since the smallest i with p^2 | i*m-1 is that
 *            inverse. To beat the current record B, m must be divisible by
 *            every prime p with p^2 <= B+2, so only multiples of...
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int128 A383548_u128;

#define A383548_PLIM 1000000u

static uint32_t *A383548_pr = NULL;
static uint64_t *A383548_pr2 = NULL;
static size_t A383548_npr = 0;

static void A383548_init_primes(void)
{
    unsigned char *s = (unsigned char *)malloc(A383548_PLIM + 1);
    size_t i, j, cnt = 0;
    memset(s, 1, A383548_PLIM + 1);
    s[0] = s[1] = 0;
    for (i = 2; i * i <= A383548_PLIM; ++i)
        if (s[i])
            for (j = i * i; j <= A383548_PLIM; j += i) s[j] = 0;
    for (i = 2; i <= A383548_PLIM; ++i) if (s[i]) ++cnt;
    A383548_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A383548_pr2 = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= A383548_PLIM; ++i)
        if (s[i]) {
            A383548_pr[cnt] = (uint32_t)i;
            A383548_pr2[cnt] = (uint64_t)i * (uint64_t)i;
            ++cnt;
        }
    A383548_npr = cnt;
    free(s);
}

/* inverse of a modulo q, gcd(a,q)=1, q <= ~1e12 */
static uint64_t A383548_inv(uint64_t a, uint64_t q)
{
    long long t = 0, nt = 1, r = (long long)q, nr = (long long)a;
    while (nr != 0) {
        long long qq = r / nr, tmp;
        tmp = t - qq * nt; t = nt; nt = tmp;
        tmp = r - qq * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (long long)q;
    return (uint64_t)t;
}

/* Returns A383546(m) if it exceeds B, otherwise 0. m >= 2. */
static uint64_t A383548_eval(A383548_u128 m, uint64_t B)
{
    uint64_t mn = 0;
    bool have = false;
    size_t j;
    for (j = 0; j < A383548_npr; ++j) {
        uint64_t q = A383548_pr2[j];
        uint64_t p = A383548_pr[j];
        uint64_t r, iv;
        if (have && (A383548_u128)q > (A383548_u128)mn * m) break;
        r = (uint64_t)(m % q);
        if (r % p == 0) continue;
        iv = A383548_inv(r, q);
        if (iv <= B + 1) return 0;
        if (!have || iv < mn) { mn = iv; have = true; }
    }
    return mn - 1;
}

static uint64_t *A383548_rec = NULL;
static size_t A383548_nrec = 0, A383548_cap = 0;
static uint64_t A383548_best = 0;
static A383548_u128 A383548_curm = 1;

seq_t a(idx_t n)
{
    if (A383548_rec == NULL) {
        A383548_init_primes();
        A383548_cap = 64;
        A383548_rec = (uint64_t *)malloc(A383548_cap * sizeof(uint64_t));
        A383548_rec[0] = 0;          /* A383546(1) = 0 */
        A383548_nrec = 1;
        A383548_best = 0;
        A383548_curm = 1;
    }
    while (A383548_nrec < n) {
        A383548_u128 st = 1, m;
        size_t j;
        for (j = 0; j < A383548_npr && A383548_pr2[j] <= A383548_best + 2; ++j)
            st *= A383548_pr[j];
        m = (A383548_curm / st + 1) * st;
        for (;; m += st) {
            uint64_t v = A383548_eval(m, A383548_best);
            if (v) {
                A383548_best = v;
                A383548_curm = m;
                if (A383548_nrec == A383548_cap) {
                    A383548_cap *= 2;
                    A383548_rec = (uint64_t *)realloc(A383548_rec,
                                        A383548_cap * sizeof(uint64_t));
                }
                A383548_rec[A383548_nrec++] = v;
                break;
            }
        }
    }
    return (seq_t)A383548_rec[n - 1];
}
