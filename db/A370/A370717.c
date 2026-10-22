/*
 * A370717  Arithmetic derivative of elements of GF(2)[x], evaluated at x=2.
 * https://oeis.org/A370717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the evaluation at x = 2 of the arithmetic derivative *
 *            of the polynomial over GF(2)[x] whose binary coefficients encode
 *            n. * The derivative is defined by (f·g)' = f'·g + f·g', with the
 *            derivative * of a monic irreducible polynomial equal to 1;
 *            repeated factors act as * units
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------- Polynomial arithmetic over GF(2) ------- */

static uint64_t poly_mul(uint64_t a, uint64_t b)
{
    uint64_t res = 0;
    while (a)
    {
        int shift = __builtin_ctzll(a);      /* position of lowest set bit */
        a &= a - 1;                          /* clear that bit */
        res ^= (uint64_t)((unsigned __int128)b << shift);
    }
    return res;
}

static int poly_deg(uint64_t p)
{
    return 63 - __builtin_clzll(p);
}

static uint64_t poly_divrem(uint64_t a, uint64_t b, uint64_t *q)
/* returns remainder of a ÷ b; if q is non‑NULL sets *q = quotient */
{
    uint64_t r = a;
    uint64_t q_ = 0;
    int deg_b = poly_deg(b);
    while (r && poly_deg(r) >= deg_b)
    {
        int shift = poly_deg(r) - deg_b;
        q_ |= (uint64_t)1 << shift;
        r ^= b << shift;
    }
    if (q) *q = q_;
    return r;
}

static bool poly_divisible(uint64_t a, uint64_t b)
{
    return poly_divrem(a, b, NULL) == 0;
}

/* ------- Irreducible polynomial table (generated on demand) ------- */

struct factor_t { uint64_t poly; unsigned exp; };

static uint64_t irr_list[2000];   /* list of monic irreducible polynomials */
static size_t irr_cnt = 0;        /* number of stored irreducibles            */
static int irr_max_deg = 0;       /* largest degree pre‑computed             */

static void ensure_irreducibles(int max_deg)
{
    if (max_deg <= irr_max_deg) return;
    for (int d = irr_max_deg + 1; d <= max_deg; ++d)
    {
        uint64_t start = 1ULL << d;
        uint64_t end   = start << 1;
        for (uint64_t cand = start; cand < end; ++cand)
        {
            bool irred = true;
            for (size_t i = 0; i < irr_cnt; ++i)
            {
                int degp = poly_deg(irr_list[i]);
                if (degp > d / 2) break;
                if (poly_divisible(cand, irr_list[i]))
                {
                    irred = false;
                    break;
                }
            }
            if (irred) irr_list[irr_cnt++] = cand;
        }
    }
    irr_max_deg = max_deg;
}

/* ------- Factorisation of a polynomial over GF(2) ------- */

static void factor_poly(uint64_t poly, struct factor_t *factors, size_t *nf)
{
    *nf = 0;
    if (poly <= 1) return;
    int max_deg = poly_deg(poly);
    ensure_irreducibles(max_deg);
    uint64_t rem = poly;
    for (size_t i = 0; i < irr_cnt; ++i)
    {
        uint64_t prime = irr_list[i];
        int degp = poly_deg(prime);
        if (degp > max_deg) break;

        unsigned exp = 0;
        while (1)
        {
            uint64_t q;
            uint64_t r = poly_divrem(rem, prime, &q);
            if (r != 0) break;
            rem = q;
            ++exp;
        }
        if (exp > 0) factors[(*nf)++] = (struct factor_t){prime, exp};
        if (rem == 1) break;
    }
}

/* ------- Sequence function ------- */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t poly = (uint64_t)n;             /* polynomial from binary of n */

    struct factor_t fac[32];
    size_t nf;
    factor_poly(poly, fac, &nf);

    uint64_t odd_primes[32];
    size_t odd_cnt = 0;
    uint64_t s2poly  = 1;                    /* product of all even parts */

    for (size_t i = 0; i < nf; ++i)
    {
        uint64_t p   = fac[i].poly;
        unsigned exp = fac[i].exp;
        unsigned sqcnt = exp / 2;
        uint64_t prime2 = poly_mul(p, p);
        for (unsigned k = 0; k < sqcnt; ++k) s2poly = poly_mul(s2poly, prime2);
        if (exp & 1) odd_primes[odd_cnt++] = p;     /* square‑free part */
    }

    uint64_t rprime = 0;                       /* derivative of square‑free part */
    if (odd_cnt)
    {
        for (size_t i = 0; i < odd_cnt; ++i)
        {
            uint64_t term = 1;
            for (size_t j = 0; j < odd_cnt; ++j)
                if (j != i) term = poly_mul(term, odd_primes[j]);
            rprime ^= term;
        }
    }

    uint64_t der_poly = poly_mul(s2poly, rprime);
    return (seq_t)der_poly;                    /* evaluation at x = 2 */
}
