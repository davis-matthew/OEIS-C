/*
 * A395776  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-23,.) mod 23.
 * https://oeis.org/A395776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative, a(23^e) = 23^e, a(p^e) = p^(e‑1)*(p‑1) if
 *            p ≡ 1,2,3,4,6,8,9,12,13,16,18 (mod 23), a(p^e) = p^(e‑1)*(p+1)
 *            otherwise
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* power base^exp using only seq_t; exp fits in idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* flags for residues giving the “-” case: 1,2,3,4,6,8,9,12,13,16,18 (mod 23) */
static const bool is_minus[23] = {
    /*0*/ false,
    /*1*/ true,
    /*2*/ true,
    /*3*/ true,
    /*4*/ true,
    /*5*/ false,
    /*6*/ true,
    /*7*/ false,
    /*8*/ true,
    /*9*/ true,
    /*10*/false,
    /*11*/false,
    /*12*/true,
    /*13*/true,
    /*14*/false,
    /*15*/false,
    /*16*/true,
    /*17*/false,
    /*18*/true,
    /*19*/false,
    /*20*/false,
    /*21*/false,
    /*22*/false
};

seq_t a(idx_t n)
{
    /* n ≥ 1; for safety return 0 for n == 0 */
    if (n == 0) return 0;

    /* factorization of n, store factors and exponents */
    uint64_t factors[64];
    idx_t     exps[64];
    idx_t     fcnt = 0;

    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            factors[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (t > 1) {
        factors[fcnt] = t;
        exps[fcnt] = 1;
        ++fcnt;
    }

    seq_t result = 1;

    for (idx_t i = 0; i < fcnt; ++i) {
        uint64_t p = factors[i];
        idx_t     e = exps[i];
        seq_t term;

        if (p == 23ULL) {
            /* a(23^e) = 23^e */
            term = pow_seq((seq_t)23, e);
        } else {
            seq_t p_pow = (e > 1) ? pow_seq((seq_t)p, e - 1) : 1;
            uint8_t r = (uint8_t)(p % 23ULL);
            if (is_minus[r])
                term = p_pow * ((seq_t)p - 1);
            else
                term = p_pow * ((seq_t)p + 1);
        }
        result *= term;
    }

    return result;
}
