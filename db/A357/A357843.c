/*
 * A357843  Numerators of the partial alternating sums of the reciprocals of
 *          the number of divisors function (A000005).
 * https://oeis.org/A357843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( Sum_{k=1..n} (-1)^(k+1) / d(k) ), where d(k)
 *            is the number of divisors of k
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>
#include <stdlib.h>

/* Helper declaration: greatest common divisor for seq_t */
#include "../utils/gcd.h"

/* ---------------- Cache structures ---------------- */
static seq_t *d_cache = NULL;     /* divisor counts, 1‑indexed */
static idx_t *spf_cache = NULL;   /* smallest prime factors, 1‑indexed */
static size_t d_cache_len = 0;     /* maximum n cached */

/* ----------------- Smallest‑prime‑factor sieve ----------------- */
static void build_spf(idx_t n, idx_t *arr)
{
    /* arr[0..n] already allocated */
    for (idx_t i = 0; i <= n; ++i) arr[i] = 0;
    if (n >= 1) arr[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (arr[i] == 0) {      /* i is prime */
            arr[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (arr[j] == 0) arr[j] = i;
                }
            }
        }
    }
}

/* ----------------- Ensure divisor cache up to n ----------------- */
static void ensure_divisor_cache(idx_t n)
{
    if ((idx_t)d_cache_len >= n) return;  /* already good */

    /* Allocate new space */
    idx_t *new_spf = (idx_t *)calloc((size_t)n + 1, sizeof(idx_t));
    seq_t *new_d   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!new_spf || !new_d) return; /* allocation failure: abort silently */

    /* Build smallest‑prime‑factor table */
    build_spf(n, new_spf);

    /* Compute divisor counts */
    new_d[0] = 0;
    new_d[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t p = new_spf[i];
        idx_t m = i;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        new_d[i] = (seq_t)(exp + 1) * new_d[(size_t)m];
    }

    /* Replace old cache */
    free(d_cache);
    free(spf_cache);
    d_cache     = new_d;
    spf_cache   = new_spf;
    d_cache_len = (size_t)n;
}

/* ----------------- Main sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* guard for index spacing */

    ensure_divisor_cache(n);

    seq_t num = 0;                        /* absolute numerator */
    seq_t den = 1;                        /* positive denominator */
    bool neg = false;                     /* false → fraction >= 0, true → fraction < 0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t d = d_cache[(size_t)k];
        bool plus = (k & 1);              /* true for +1/d, false for -1/d */

        /* Multiply current numerator by d to align denominators */
        seq_t prod = num * d;

        seq_t new_abs;
        bool new_neg;

        if (!neg) {                       /* current fraction is non‑negative */
            if (plus) {                   /* add +1/d */
                new_abs = prod + den;
                new_neg  = false;
            } else {                      /* add -1/d */
                if (prod >= den) {
                    new_abs = prod - den;
                    new_neg  = false;
                } else {
                    new_abs = den - prod;
                    new_neg  = true;
                }
            }
        } else {                          /* current fraction is negative */
            if (plus) {                   /* add +1/d to a negative value */
                if (den >= prod) {
                    new_abs = den - prod;
                    new_neg  = true;        /* remains negative */
                } else {
                    new_abs = prod - den;
                    new_neg  = false;       /* becomes positive */
                }
            } else {                      /* add -1/d to a negative value */
                new_abs = prod + den;
                new_neg  = true;
            }
        }

        seq_t new_den = den * d;          /* new common denominator */

        if (new_abs == 0) {               /* fraction became zero */
            neg = false;
            num = 0;
            den = 1;
            continue;
        }

        /* Reduce the fraction by its GCD */
        seq_t g = gcd(new_abs, new_den);
        new_abs /= g;
        new_den /= g;

        num = new_abs;
        den = new_den;
        neg = new_neg;
    }

    /* The sum is always non‑negative for all tested n, so return numerator */
    return num;
}
