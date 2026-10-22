/*
 * A303989  Triangle read by rows: denominators of c_{n,k}, n >= 0, k = 0..n,
 *          used in the proof that Zeta(3) is irrational.
 * https://oeis.org/A303989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) is the denominator of the rational number c_{i,j} * *
 *            c_{i,j} = ζ(3,i) + Σ_{m=1..j} (-1)^(m-1) / (2*m^3*B(i,m)), *
 *            where ζ(3,i) = Σ_{k=1..i} 1/k^3 and * B(i,m) = binomial(i+m,m) *
 *            binomial(i,m). * * The triangle T(i,j) = denominator(c_{i,j}) is
 *            read row‑wise. * a(0) = T(0,0), a(1) = T(1,0),...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

/* static cache for memoization */
static seq_t *cache = NULL;         /* array of denominators */
static size_t cache_len = 0;        /* number of computed entries */
static size_t cache_cap = 0;        /* allocated capacity */
static idx_t next_row = 0;          /* next row to compute */

/* previous Zeta(3) value */
static seq_t prev_zeta_num = 0;
static seq_t prev_zeta_den = 1;

/* ensure cache contains at least indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache && cache_len > (size_t)n) return;  /* already present */

    /* initial allocation */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* compute rows until we have enough entries */
    for (idx_t r = next_row; r <= n; ++r) {
        seq_t cur_zeta_num, cur_zeta_den;

        if (r == 0) {
            /* Zeta(3,0) = 0/1, but T(0,0) is defined as 1 */
            cur_zeta_num = 0;
            cur_zeta_den = 1;
            /* enlarge cache if needed */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = 1;          /* T(0,0) = 1 */
        } else {
            /* compute Zeta(3,r) from previous value */
            seq_t r3 = (seq_t)r * r * r;                                 /* r^3 */
            seq_t num = seq_mul(prev_zeta_num, r3) + prev_zeta_den;   /* prev_num*r^3 + prev_den */
            seq_t den = seq_mul(prev_zeta_den, r3);
            seq_t g = gcd(num, den);
            cur_zeta_num = num / g;
            cur_zeta_den = den / g;

            /* store updated zeta for next iteration */
            prev_zeta_num = cur_zeta_num;
            prev_zeta_den = cur_zeta_den;

            /* current fraction before any B terms added */
            seq_t cur_num = cur_zeta_num;
            seq_t cur_den = cur_zeta_den;

            /* k = 0 entry */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = cur_den;

            /* add terms for m = 1 .. r  (k = 1 .. r) */
            for (idx_t m = 1; m <= r; ++m) {
                int sign = (m & 1) ? 1 : -1;          /* (-1)^(m-1) */

                seq_t m3 = (seq_t)m * m * m;          /* m^3 */
                seq_t B1 = binomial(r + m, m);       /* binomial(r+m, m) */
                seq_t B2 = binomial(r, m);           /* binomial(r,   m) */
                seq_t Bnm = seq_mul(B1, B2);          /* B(r,m) = B1 * B2 */

                seq_t term_den = seq_mul(seq_mul(m3, (seq_t)2), Bnm);  /* 2 * m^3 * B(r,m) */

                seq_t num_mul_term = seq_mul(cur_num, term_den);
                seq_t new_num;
                if (sign > 0)
                    new_num = num_mul_term + cur_den;
                else
                    new_num = num_mul_term - cur_den; /* subtraction is safe */

                seq_t new_den = seq_mul(cur_den, term_den);
                seq_t g2 = gcd(new_num, new_den);

                cur_num = new_num / g2;
                cur_den = new_den / g2;

                /* store denominator for this k */
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(1);
                }
                cache[cache_len++] = cur_den;
            }
        }
    }
    next_row = n + 1;  /* next time start after this row */
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
