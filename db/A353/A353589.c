/*
 * A353589  Number of nondecreasing nonnegative integer quadruples (m,p,q,r)
 *          such that m^2 + p^2 + q^2 + r^2 = n^2 and m +- p +- q +- r = +- n.
 * https://oeis.org/A353589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts non‑decreasing quadruples (m,p,q,r) of non‑negative integers
   such that m²+p²+q²+r² = n² and m ± p ± q ± r = ± n.
   It follows the definition of OEIS sequence A353589. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 32‑bit unsigned value.
   Returns floor(sqrt(v)) using a bit‑shift algorithm. */
static uint32_t isqrt_u32(uint32_t v)
{
    uint32_t r = 0, bit = 1u << 30;          /* 2^30 is the largest square < 2^32 */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Structure to keep a found quadruple while eliminating duplicates. */
typedef struct { idx_t m, p, q, r; } Quad;

/* Main sequence function. */
seq_t a(idx_t n)
{
    int64_t n64 = (int64_t)n;
    int64_t n2  = n64 * n64;          /* n², fits in 64‑bit for n ≤ 10 000 */

    /* Pre‑compute squares of numbers 0..n (fits in 32‑bit). */
    uint32_t *sq = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!sq) return 0;                /* allocation failure – unlikely */
    for (idx_t i = 0; i <= n; ++i) sq[i] = (uint32_t)(i * i);

    /* Dynamic array for unique solutions. */
    size_t sol_cap = 64, sol_len = 0;
    Quad *solutions = (Quad *)malloc(sol_cap * sizeof(Quad));
    if (!solutions) { free(sq); return 0; }

    /* Enumerate m and p with m ≤ p and m²+p² ≤ n². */
    for (idx_t m = 0; m <= n; ++m) {
        uint32_t m2 = sq[m];
        uint32_t maxP_sq = (uint32_t)(n2 - m2);          /* non‑negative */
        idx_t   p_max  = (idx_t)isqrt_u32(maxP_sq);

        for (idx_t p = m; p <= p_max; ++p) {
            uint32_t p2 = sq[p];

            /* Signs for ε1 (A) and ε4 (D).  4 combinations. */
            for (int A = -1; A <= 1; A += 2) {
                for (int D = -1; D <= 1; D += 2) {
                    int64_t k = (int64_t)D * n64 - (int64_t)m - (int64_t)A * (int64_t)p;
                    int64_t k2 = k * k;
                    int64_t Dq = 2 * n2 - (k2 + 2LL * m2 + 2LL * p2);
                    if (Dq < 0) continue;                 /* discriminant negative */

                    uint64_t sqrtD = isqrt_u32((uint32_t)Dq);
                    if (sqrtD * sqrtD != (uint64_t)Dq) continue;   /* not a perfect square */

                    /* Sign for ε2 (B) – two possibilities. */
                    for (int B = 1; B >= -1; B -= 2) {
                        int64_t base = (int64_t)B * k;          /* B*k */

                        /* Two roots from quadratic formula: ± sqrtD. */
                        for (int s = 1; s >= -1; s -= 2) {
                            int64_t numer = base + s * (int64_t)sqrtD;
                            if ((numer & 1) != 0) continue;     /* q would not be integer */
                            int64_t q = numer / 2;
                            if (q < (int64_t)p || q > (int64_t)n) continue;

                            int64_t r = llabs(k - (int64_t)B * q);
                            if (r < (int64_t)q || r > (int64_t)n) continue;

                            /* Store unique quadruple (m,p,q,r). */
                            idx_t qm = (idx_t)m, qp = (idx_t)p, qq = (idx_t)q, qr = (idx_t)r;
                            bool dup = false;
                            for (size_t i = 0; i < sol_len; ++i) {
                                if (solutions[i].m == qm &&
                                    solutions[i].p == qp &&
                                    solutions[i].q == qq &&
                                    solutions[i].r == qr) {
                                    dup = true;
                                    break;
                                }
                            }
                            if (!dup) {
                                if (sol_len == sol_cap) {
                                    sol_cap <<= 1;
                                    solutions = (Quad *)realloc(solutions,
                                        sol_cap * sizeof(Quad));
                                }
                                solutions[sol_len++] = (Quad){qm, qp, qq, qr};
                            }
                        }
                    }
                }
            }
        }
    }

    free(sq);
    free(solutions);

    return (seq_t)sol_len;
}
