/*
 * A345692  For 1<=x<=n, 1<=y<=n with gcd(x,y)=1, write 1 = gcd(x,y) = u*x+v*y
 *          with u,v minimal; a(n) = m^2*s, where s is the population variance
 *          of the values of u and m is the number of such values.
 * https://oeis.org/A345692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute A345692: a(n) = m^2 * s where 
 *   - m = number of coprime pairs (x,y) with 1≤x,y≤n
 *   - For each pair solve 1 = u*x + v*y with u minimal in absolute value
 *   - s = population variance of the u’s
 *
 * The implementation sums Σu and Σu² over all pairs and returns 
 *                     m·Σu² – (Σu)²
 * using the minimal‑absolute‑value representation of u.
 */

#include <stdint.h>
#include <stdlib.h>

/* Euclidean GCD for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Extended Euclidean algorithm to compute the inverse of a modulo m. */
static inline seq_t modinv(idx_t a, idx_t m) {
    int64_t t = 0, newt = 1;
    int64_t r = m, newr = a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    /* r == 1 */
    if (t < 0) t += m;
    return (seq_t)t;
}

seq_t a(idx_t n) {
    /* Pre‑compute φ(y) for 1 ≤ y ≤ n */
    uint32_t *phi = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!phi) return (seq_t)0;          /* unlikely to happen */

    for (idx_t i = 0; i <= n; ++i) phi[i] = (uint32_t)i;
    for (idx_t i = 2; i <= n; ++i) {
        if (phi[i] == (uint32_t)i) {           /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                phi[j] -= phi[j] / (uint32_t)i;
        }
    }
    phi[1] = 1;

    int64_t sumU = 0;           /* Σu */
    int64_t sumU2 = 0;          /* Σu² */
    int64_t m = 0;              /* number of coprime pairs */

    for (idx_t y = 1; y <= n; ++y) {
        if (y == 1) {
            m += n;              /* every x (1≤x≤n) is coprime to 1, u=0 */
            continue;
        }

        /* Count of x in 1..n that are coprime to y */
        idx_t full = n / y;
        idx_t rem  = n % y;
        idx_t base_count = phi[y];
        idx_t remaining = 0;
        for (idx_t r = 1; r <= rem; ++r)
            if (gcd_idx(r, y) == 1)
                ++remaining;
        idx_t total_count = full * base_count + remaining;
        m += total_count;

        /* Iterate over residues r = 1 … y-1 that are coprime to y */
        for (idx_t r = 1; r < y; ++r) {
            if (gcd_idx(r, y) != 1) continue;

            idx_t cnt_r = full + (r <= rem ? 1 : 0);

            /* modular inverse of r modulo y */
            int64_t inv = (int64_t)modinv(r, y);    /* 0 ≤ inv < y */

            /* minimal‑absolute‑value representation of u */
            int64_t half = (int64_t)(y >> 1);
            int64_t signed_u = inv;
            if (inv > half) signed_u = inv - (int64_t)y;

            sumU  += signed_u * (int64_t)cnt_r;
            sumU2 += signed_u * signed_u * (int64_t)cnt_r;
        }
    }

    free(phi);

    /* a(n) = m * Σu² – (Σu)² */
    seq_t m_seq       = (seq_t)m;
    seq_t sumU2_seq   = (seq_t)sumU2;
    seq_t sumU_abs    = (seq_t)(sumU >= 0 ? sumU : -sumU);
    seq_t prod1 = m_seq * sumU2_seq;
    seq_t prod2 = sumU_abs * sumU_abs;
    return prod1 - prod2;
}
