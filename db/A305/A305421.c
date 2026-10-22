/*
 * A305421  GF(2)[X] factorization prime shift towards larger terms.
 * https://oeis.org/A305421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = carry‑less product of the next irreducible * polynomials
 *            in the GF(2)[X] factorization of the integer n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9908, u64 n <= 9908,
 *            u128 n <= 9908, big any n
 * Helpers:   carry_mul_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/carry_mul_seq.h"

/* ------------------------------------------------------------------ */
/*  Precomputed list of irreducible monic polynomials over GF(2).      */
/*  Each element is stored as the binary representation of the polynomial.
 *  irrVals[1] corresponds to the first irreducible (x), irrVals[2] to
 *  the second (x+1), and so on.                                            */
/* ------------------------------------------------------------------ */
#define MAX_IRR 5000            /* ample for all n up to 2^64-1 */
static uint64_t irrVals[MAX_IRR];   /* 1‑based: irrVals[1] = 2 */
static int      irrDegs[MAX_IRR];
static size_t   irrSize = 1; /* start with dummy at index 0 */

/* Degree of a non‑zero unsigned 64‑bit integer. */
static inline int deg_uint64(uint64_t x)
{
    return x ? (63 - __builtin_clzll(x)) : -1;
}

/* Polynomial division over GF(2).  Returns quotient; remainder
 * stored in *rem if rem != NULL. */
static inline uint64_t poly_div(uint64_t a, uint64_t b, uint64_t *rem)
{
    uint64_t q = 0;
    uint64_t r = a;
    int degb = deg_uint64(b);
    while (r != 0 && deg_uint64(r) >= degb) {
        int shift = deg_uint64(r) - degb;
        r ^= b << shift;
        q |= 1ULL << shift;
    }
    if (rem) *rem = r;
    return q;
}

/* Generate all monic irreducible polynomials over GF(2) up to degree
 * maxDeg (inclusive).  The list is expanded lazily. */
static void gen_irreducibles_up_to_deg(int maxDeg)
{
    static int maxGenDeg = 0;
    while (maxGenDeg < maxDeg) {
        ++maxGenDeg;
        uint64_t start = 1ULL << maxGenDeg;
        uint64_t end   = 1ULL << (maxGenDeg + 1);
        for (uint64_t val = start; val < end; ++val) {
            int degHalf = maxGenDeg / 2;
            unsigned int i;
            int isIrreducible = 1;
            for (i = 1; i < irrSize; ++i) {
                if (irrDegs[i] > degHalf) break;
                uint64_t r;
                poly_div(val, irrVals[i], &r);
                if (r == 0) { isIrreducible = 0; break; }
            }
            if (isIrreducible) {
                if (irrSize >= MAX_IRR) return; /* safety net */
                irrVals[irrSize] = val;
                irrDegs[irrSize] = maxGenDeg;
                ++irrSize;
            }
        }
    }
}

/* Return the index (1‑based) of val in irrVals or 0 if not found. */
static int index_of_value(uint64_t val)
{
    for (size_t i = 1; i < irrSize; ++i)
        if (irrVals[i] == val)
            return (int)i;
    return 0;
}

/* Recursive factorisation of polynomial f into irreducible indices.
 * Indices stored in out array; *len is updated. */
static void factor_rec(uint64_t f, int *out, size_t *len)
{
    if (f == 1) return;
    int idx = index_of_value(f);
    if (idx > 0) {                       /* f is irreducible */
        out[(*len)++] = idx;
        return;
    }
    int degF    = deg_uint64(f);
    int degHalf = degF / 2;
    for (size_t i = 1; i < irrSize; ++i) {
        if (irrDegs[i] > degHalf) break;
        uint64_t r;
        uint64_t q = poly_div(f, irrVals[i], &r);
        if (r == 0) {                   /* divisible by irrVals[i] */
            factor_rec(irrVals[i], out, len);
            factor_rec(q,          out, len);
            return;
        }
    }
    /* For n from OEIS the input is always factorable.  */
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;                    /* idx_t → native */
    int degN   = deg_uint64(nn);
    gen_irreducibles_up_to_deg(degN + 1);          /* ensure we have enough irreds */

    int factorIdx[64];
    size_t factorCnt = 0;
    factor_rec(nn, factorIdx, &factorCnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < factorCnt; ++i) {
        seq_t val = (seq_t)irrVals[factorIdx[i] + 1];  /* next irreducible */
        res = carry_mul_seq(res, val);
    }
    return res;
}
