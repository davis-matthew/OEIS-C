/*
 * A381241  Records in A381096.
 * https://oeis.org/A381241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value of A381096(m) = m - phi(m) -
 *            A005361(m), where A005361(m) is the product of the prime
 *            exponents of m. Computed by a segmented sieve over m that tracks
 *            the running maximum (m = 1 gives a(1) = 0)
 * Range:     u8 n <= 36, u16 n <= 317, u32 n <= 1782, u64 n <= 1782,
 *            u128 n <= 1782, big any n
 * Validated: standard, pass, 74 terms through n = 1720 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define A381241_B 65536

static uint64_t *A381241_rec = NULL;
static size_t A381241_cnt = 0, A381241_cap = 0;
static uint64_t A381241_lo = 2, A381241_max = 0;
static bool A381241_init = false;

static uint64_t *A381241_pr = NULL;
static size_t A381241_np = 0;
static uint64_t A381241_plim = 0;

static uint64_t A381241_found[A381241_B];
static uint64_t A381241_ph[A381241_B];
static uint32_t A381241_tu[A381241_B];

static bool A381241_ensure_primes(uint64_t lim)
{
    if (A381241_plim >= lim)
        return true;
    uint64_t nl = lim;
    if (nl < 2 * A381241_plim)
        nl = 2 * A381241_plim;
    if (nl < 1024)
        nl = 1024;
    unsigned char *s = (unsigned char *)malloc((size_t)nl + 1);
    if (!s)
        return false;
    memset(s, 1, (size_t)nl + 1);
    s[0] = s[1] = 0;
    for (uint64_t i = 2; i * i <= nl; ++i)
        if (s[i])
            for (uint64_t j = i * i; j <= nl; j += i)
                s[j] = 0;
    size_t c = 0;
    for (uint64_t i = 2; i <= nl; ++i)
        if (s[i])
            ++c;
    uint64_t *np_ = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
    if (!np_)
    {
        free(s);
        return false;
    }
    c = 0;
    for (uint64_t i = 2; i <= nl; ++i)
        if (s[i])
            np_[c++] = i;
    free(s);
    free(A381241_pr);
    A381241_pr = np_;
    A381241_np = c;
    A381241_plim = nl;
    return true;
}

static bool A381241_push(uint64_t v)
{
    if (A381241_cnt == A381241_cap)
    {
        size_t nc = A381241_cap ? A381241_cap * 2 : 1024;
        uint64_t *nr = (uint64_t *)realloc(A381241_rec, nc * sizeof(uint64_t));
        if (!nr)
            return false;
        A381241_rec = nr;
        A381241_cap = nc;
    }
    A381241_rec[A381241_cnt++] = v;
    return true;
}

static bool A381241_block(void)
{
    uint64_t lo = A381241_lo;
    uint64_t hi = lo + A381241_B - 1;
    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= hi)
        ++sq;
    if (!A381241_ensure_primes(sq))
        return false;

    for (size_t i = 0; i < A381241_B; ++i)
    {
        A381241_found[i] = 1;
        A381241_ph[i] = 1;
        A381241_tu[i] = 1;
    }

    for (size_t t = 0; t < A381241_np; ++t)
    {
        uint64_t p = A381241_pr[t];
        if (p > sq)
            break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t j = st - lo; j < A381241_B; j += p)
        {
            A381241_found[j] *= p;
            A381241_ph[j] *= (p - 1);
        }
        uint64_t pk = p;
        uint32_t k = 1;
        while (pk <= hi / p)
        {
            pk *= p;
            ++k;
            st = ((lo + pk - 1) / pk) * pk;
            for (uint64_t j = st - lo; j < A381241_B; j += pk)
            {
                A381241_found[j] *= p;
                A381241_ph[j] *= p;
                A381241_tu[j] = A381241_tu[j] / (k - 1) * k;
            }
        }
    }

    for (size_t i = 0; i < A381241_B; ++i)
    {
        uint64_t m = lo + i;
        uint64_t r = m / A381241_found[i];
        uint64_t phi = A381241_ph[i];
        if (r > 1)
            phi *= (r - 1);
        uint64_t v = m - phi - (uint64_t)A381241_tu[i];
        if (v > A381241_max)
        {
            A381241_max = v;
            if (!A381241_push(v))
                return false;
        }
    }
    A381241_lo = hi + 1;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    if (!A381241_init)
    {
        if (!A381241_push(0)) /* m = 1: A381096(1) = 0 */
            return 0;
        A381241_max = 0;
        A381241_lo = 2;
        A381241_init = true;
    }
    while (A381241_cnt < n)
    {
        if (!A381241_block())
            return 0;
    }
    return (seq_t)A381241_rec[n - 1];
}
