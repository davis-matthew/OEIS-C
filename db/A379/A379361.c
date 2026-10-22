/*
 * A379361  Numerators of the partial alternating sums of the reciprocals of
 *          the number of abelian groups function (A000688).
 * https://oeis.org/A379361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the partial alternating sum Sum_{k=1..n}
 *            (-1)^(k+1) / A000688(k), where A000688(k) is the number of
 *            abelian groups of order k
 * Range:     u8 n <= 23, u16 n <= 207, u32 n <= 8182, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

/* ---- static helpers ---- */

static idx_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_cap = 0;

static seq_t *abelian = NULL;      /* cache of A000688 values */
static size_t abelian_cap = 0;

/* 64‑bit partition table (enough for exponents up to 64) */
#define PART_MAX 64
static seq_t part_tbl[PART_MAX + 1];
static bool part_inited = false;

static void init_part_table(void)
{
    if (part_inited) return;
    part_tbl[0] = 1;
    for (idx_t n = 1; n <= PART_MAX; ++n) {
        seq_t total = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            seq_t sign = (k & 1) ? 1 : -1;
            if (sign == 1) total += part_tbl[n - pent1];
            else           total -= part_tbl[n - pent1];
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= n) {
                if (sign == 1) total += part_tbl[n - pent2];
                else           total -= part_tbl[n - pent2];
            }
        }
        part_tbl[n] = total;
    }
    part_inited = true;
}

/* Ensure SPF array up to |limit| */
static void ensure_spf(idx_t limit)
{
    if (spf_cap > limit) return;
    free(spf);
    spf = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf[1] = 1;
    spf_cap = limit;
}

/* Compute A000688(n) using precomputed SPF and partition table */
static seq_t compute_abelian(idx_t n)
{
    seq_t res = 1;
    idx_t tmp = n;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        idx_t exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }
        res *= part_tbl[exp];
    }
    return res;
}

/* Ensure abelian cache up to n */
static void ensure_abelian(idx_t n)
{
    if (abelian_cap > n) return;
    abelian = (seq_t*)realloc(abelian, (n + 1) * sizeof(seq_t));
    for (idx_t i = abelian_cap; i <= n; ++i) {
        abelian[i] = compute_abelian(i);
    }
    abelian_cap = n + 1;
}

/* ---- sequence function ---- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset 1: undefined for n=0 */

    init_part_table();
    ensure_spf(n);
    ensure_abelian(n);

    seq_t num = 0;             /* absolute numerator */
    seq_t den = 1;             /* denominator */
    int sign = 0;              /* 0 for zero, +1 or -1 otherwise */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak = abelian[k];
        int termSign = (k & 1) ? 1 : -1;   /* (-1)^(k+1) */

        if (sign == 0) {                    /* first non‑zero term */
            num = 1;
            den = ak;
            sign = termSign;
            continue;
        }

        seq_t left = num * ak;   /* |current| * ak */
        seq_t right = den;       /* current denominator */
        seq_t newAbs;
        int newSign;

        if (sign == termSign) {
            newSign = sign;
            newAbs = left + right;
        } else {
            if (left > right) {
                newSign = sign;
                newAbs = left - right;
            } else if (right > left) {
                newSign = termSign;
                newAbs = right - left;
            } else {
                newSign = 0;
                newAbs = 0;
            }
        }

        seq_t newDen = den * ak;

        /* Reduce by gcd */
        if (newAbs != 0) {
            seq_t g = gcd(newAbs, newDen);
            if (g > 1) {
                newAbs /= g;
                newDen /= g;
            }
        } else {
            newDen = 1;   /* keep denominator defined for zero */
        }

        num = newAbs;
        den = newDen;
        sign = newSign;
    }

    /* The sequence lists the absolute numerators (all non‑negative). */
    return num;
}
