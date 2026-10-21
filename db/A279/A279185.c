/*
 * A279185  Triangle read by rows: T(n,k) (n >= 1, 0 <= k <= n-1) is the
 *          length of the period of the sequence obtained by starting with k
 *          and repeatedly squaring mod n.
 * https://oeis.org/A279185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the terms of OEIS A279185 (triangle read by rows). For
 *            each n we find the row r and column k. A(r,k) = ord'(2, ord'(k,
 *            r)), where ord'(x, m) = the multiplicative order of x modulo the
 *            largest divisor of m that is coprime to x
 * Range:     u8 n <= 8995, u16 n <= 8995, u32 n <= 8995, u64 n <= 8995,
 *            u128 n <= 8995, big any n
 * Helpers:   coprime_divisor, gcd, mod_pow, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers (seq_t and idx_t are defined by the build). */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/coprime_divisor.h"
#include "../utils/odd_part.h"

/* Return multiplicative order of base modulo mod.
   mod > 1 and gcd(base, mod) == 1 is assumed. */
static seq_t multiplicative_order(seq_t base, seq_t mod)
{
    if (mod <= 1)
        return 1;                 /* by convention */

    /* Compute Euler's totient φ(mod). */
    seq_t phi = mod;
    seq_t t = mod;
    seq_t p = 2;
    while (p * p <= t) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            phi = phi / p * (p - 1);
        }
        ++p;
    }
    if (t > 1)
        phi = phi / t * (t - 1);

    /* Factor φ to obtain distinct prime factors. */
    seq_t pf[64];
    size_t pf_cnt = 0;
    t = phi;
    p = 2;
    while (p * p <= t) {
        if (t % p == 0) {
            pf[pf_cnt++] = p;
            while (t % p == 0) t /= p;
        }
        ++p;
    }
    if (t > 1 && pf_cnt < 64)
        pf[pf_cnt++] = t;

    /* Reduce the order using the prime factors. */
    seq_t order = phi;
    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t f = pf[i];
        while (order % f == 0) {
            seq_t cand = order / f;
            if (mod_pow(base, (uint64_t)cand, mod) == 1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

/* Return the odd part of x (divide by all powers of 2). */

/* Triangle entry T(n,k) as defined in the OEIS entry. */
static seq_t triangle_T(seq_t n, seq_t k)
{
    if (k == 0 || k == 1)
        return 1;                     /* trivial periods */

    /* d = largest divisor of n coprime to k */
    seq_t d = coprime_divisor(n, k);
    if (d <= 1)  /* e.g., n=32,k=18 -> d=1 */
        return 1;

    seq_t base_k = k % d;
    seq_t ord_k = multiplicative_order(base_k, d);

    /* Compute ord'(2, ord_k) : remove all 2‑factors from ord_k */
    seq_t m2 = odd_part(ord_k);
    if (m2 <= 1)
        return 1;

    return multiplicative_order((seq_t)2, m2);
}

/* Convert flattened index n to triangle coordinates (r,k). */
static void index_to_row_col(idx_t n, idx_t *r, idx_t *k)
{
    /* Find smallest r with r*(r+1)/2 >= n. */
    long double sq = sqrt((long double)(8 * (long double)n + 1));
    idx_t rr = (idx_t)((sq - 1) / 2);
    /* Adjust in case of rounding errors. */
    while ((unsigned __int128)rr * (rr + 1) / 2 < n) ++rr;
    while (rr > 1 && ((unsigned __int128)(rr - 1) * rr / 2) >= n) --rr;

    idx_t prev_sum = ((unsigned __int128)(rr - 1) * rr) / 2;
    *r = rr;
    *k = n - prev_sum - 1;
}

/* Public OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined below offset */

    idx_t r, k;
    index_to_row_col(n, &r, &k);
    return triangle_T((seq_t)r, (seq_t)k);
}
