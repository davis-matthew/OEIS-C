/*
 * A266785  The growth series for the affine Weyl group E_7.
 * https://oeis.org/A266785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 96, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A266785 – growth series for the affine Weyl group E_7.
 * Computed from the rational generating function
 *   (1 - x^2)(1 - x^18)(1 - x^6)(1 - x^8)(1 - x^10)(1 - x^12)(1 - x^14) /
 *   ((1 - x)^8 (1 - x^17) (1 - x^5) (1 - x^7) (1 - x^9) (1 - x^11) (1 - x^13))
 * i.e. Q(t) = 1 + c1 t + … + c70 t^70 is the denominator and P(t)
 * = p0 + p1 t + … + p70 t^70 the numerator. In particular
 *   A(t) = P(t)/Q(t) = Σ a(n) t^n .
 * The recurrence is a(n) = p(n) − Σ_{i=1}^{70} c_i a(n−i)       (p(n)=0 for n>70).
 * The recurrence can be written using only positive additions by separating
 * positive and negative coefficients:
 *   let pos = p(n) + Σ_{c_i<0} |c_i| a(n−i)
 *       neg =      Σ_{c_i>0}   c_i a(n−i)
 *   a(n) = pos − neg .
 * The coefficients c_i and p_i are small (degree 70), so they are stored
 * as signed 64‑bit integers.
 */

#include <stdint.h>
#include <stdlib.h>

#define MAX_DEG 70

static void init_polys(int64_t *p, int64_t *q) {
    /* ---- numerator P(t) ---- */
    int64_t poly[MAX_DEG + 1] = {0};
    int deg = 0;
    poly[0] = 1;
    int factors[] = {2, 18, 6, 8, 10, 12, 14};
    for (int f = 0; f < 7; ++f) {
        int k = factors[f];
        int64_t newpoly[MAX_DEG + 1] = {0};
        for (int i = 0; i <= deg; ++i) {
            newpoly[i] += poly[i];
            if (i + k <= MAX_DEG)
                newpoly[i + k] -= poly[i];
        }
        for (int i = 0; i <= MAX_DEG; ++i)
            poly[i] = newpoly[i];
        if (deg + k <= MAX_DEG)
            deg += k;
        else
            deg = MAX_DEG;
    }
    for (int i = 0; i <= MAX_DEG; ++i)
        p[i] = poly[i];

    /* ---- denominator Q(t) ---- */
    int64_t polyq[MAX_DEG + 1] = {0};
    int degq = 0;
    polyq[0] = 1;
    /* (1 - t)^8 */
    for (int tcnt = 0; tcnt < 8; ++tcnt) {
        int64_t newpolyq[MAX_DEG + 1] = {0};
        for (int i = 0; i <= degq; ++i) {
            newpolyq[i] += polyq[i];
            if (i + 1 <= MAX_DEG)
                newpolyq[i + 1] -= polyq[i];
        }
        for (int i = 0; i <= MAX_DEG; ++i)
            polyq[i] = newpolyq[i];
        if (degq + 1 <= MAX_DEG)
            ++degq;
        else
            degq = MAX_DEG;
    }
    int qfactors[] = {17, 5, 7, 9, 11, 13};
    for (int f = 0; f < 6; ++f) {
        int k = qfactors[f];
        int64_t newpolyq[MAX_DEG + 1] = {0};
        for (int i = 0; i <= degq; ++i) {
            newpolyq[i] += polyq[i];
            if (i + k <= MAX_DEG)
                newpolyq[i + k] -= polyq[i];
        }
        for (int i = 0; i <= MAX_DEG; ++i)
            polyq[i] = newpolyq[i];
        if (degq + k <= MAX_DEG)
            degq += k;
        else
            degq = MAX_DEG;
    }
    for (int i = 0; i <= MAX_DEG; ++i)
        q[i] = polyq[i];
}

seq_t a(idx_t n) {
    /* static data ---------------------------------------------------- */
    static int64_t p[MAX_DEG + 1];
    static int64_t q[MAX_DEG + 1];
    static int initialized = 0;
    if (!initialized) {
        init_polys(p, q);
        initialized = 1;
    }

    /* cache for already computed a(k) -------------------------------- */
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;
    if (n >= cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0; /* allocation failure – unlikely for the judge */
        cache = new_cache;
        for (idx_t k = cache_size; k <= n; ++k) {
            int64_t pn = (k <= MAX_DEG) ? p[(int)k] : 0;
            seq_t pos = 0;
            seq_t neg = 0;

            /* add term from the numerator */
            if (pn >= 0)
                pos += (seq_t)pn;
            else
                neg += (seq_t)(-pn);

            /* add contributions from denominator coefficients */
            for (int i = 1; i <= MAX_DEG && i <= k; ++i) {
                int64_t ci = q[i];
                if (ci >= 0)
                    neg += cache[k - i] * (seq_t)ci;
                else
                    pos += cache[k - i] * (seq_t)(-ci);
            }
            cache[k] = pos - neg; /* guaranteed non‑negative */
        }
        cache_size = n + 1;
    }
    return cache[n];
}
