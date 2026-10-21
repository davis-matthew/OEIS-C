/*
 * A285989  a(0) = 0, a(n) = Sum_{0<d|n, n/d odd} d^4 for n > 0.
 * https://oeis.org/A285989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{0<d|n, n/d odd} d^4 (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* ---------- helper for multiplication without overflow (for uint64_t) ---------- */
static inline seq_t mul128(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((__uint128_t)a) * b);
#else
    /* Fallback – assumes that a*b will not overflow in the test range. */
    return a * b;
#endif
}

/* ---------- SPF table handling ---------- */
static idx_t spf_limit = 0;
static idx_t *spf = NULL;

static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;
    idx_t *new_spf = NULL;
    build_spf(n, &new_spf);          /* allocates a new SPF table */
    if (spf)
        free(spf);
    spf = new_spf;
    spf_limit = n;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* factor out powers of 2: n = 2^e * m,  m odd */
    idx_t e = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++e;
    }
    idx_t m = tmp;                     /* odd part */

    ensure_spf(n);

    /* 2^(4e) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < e; ++i)
        pow2 = mul128(pow2, 16);

    /* σ₄(m) for odd m */
    seq_t sigma4 = 1;
    idx_t x = m;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }

        /* compute 1 + p^4 + p^8 + … + p^(4·cnt) */
        seq_t p2 = mul128((seq_t)p, (seq_t)p);          /* p² */
        seq_t p4 = mul128(p2, p2);                      /* p⁴ */

        seq_t term = 1;
        seq_t sum  = 1;
        for (idx_t i = 1; i <= cnt; ++i) {
            term = mul128(term, p4);                    /* p^(4·i) */
            sum  = (seq_t)(((__uint128_t)sum + term));  /* safe addition */
        }
        sigma4 = mul128(sigma4, sum);
    }

    return mul128(pow2, sigma4);
}
