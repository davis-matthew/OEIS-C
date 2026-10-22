/*
 * A337991  Triangle read by rows: T(n,m) = Sum_{i=1..n}
 *          C(n,i-m)*C(n+m-i,i-1)*C(n+m-i,m)/n, with T(0,0)=1.
 * https://oeis.org/A337991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{i=1..r} T(r,i-1) where T(r,m)=∑_{s=1..r}
 *            C(r,s-m)·C(r+m-s,s-1)·C(r+m-s,m)/r and a(0)=1. The triangle is
 *            read row‑by‑row
 * Range:     u8 n <= 28, u16 n <= 68, u32 n <= 193, u64 n <= 571,
 *            u128 n <= 1972, big any n
 * Helpers:   exp_in_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/exp_in_num.h"

static int   primes[20050];
static int   prime_cnt = 0;
static idx_t primes_max_gen = 0;

/* ------------------------------------------------------------
 * Generate all primes ≤ limit  (limit ≤ ~20000, cheap).
 * ------------------------------------------------------------ */
static void generate_primes(idx_t limit)
{
    if (limit <= primes_max_gen) return;
    int L = (int)limit;
    bool *comp = (bool*)calloc(L + 1, sizeof(bool));
    if (!comp) return;
    prime_cnt = 0;
    for (int i = 2; i <= L; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((int64_t)i * i <= L) {
                for (int j = i * i; j <= L; j += i) comp[j] = true;
            }
        }
    }
    free(comp);
    primes_max_gen = limit;
}

/* ------------------------------------------------------------
 * Exponent of prime p in n!  (Legendre’s formula).
 * ------------------------------------------------------------ */
static idx_t exp_in_fact(idx_t n, int p)
{
    idx_t e = 0;
    while (n) { n /= p; e += n; }
    return e;
}

/* ------------------------------------------------------------
 * Compute the triangle entry T(n,m)  (n,m ≥ 0, m ≤ n).
 * ------------------------------------------------------------ */
static seq_t compute_T(idx_t n, idx_t m)
{
    if (n == 0) return (seq_t)1;

    generate_primes(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i < m) continue;                     /* k < 0 -> term 0 */
        idx_t k = i - m;                        /* k = i – m  */
        if (k > n) continue;                    /* impossible but keep guard */
        if (2 * k + m > n + 1) continue;        /* binomial (n+m-i choose i-1) = 0 */

        seq_t term = 1;
        bool zero = false;

        for (int pi = 0; pi < prime_cnt; ++pi) {
            int p = primes[pi];

            /* compute exponents of each prime in the product */
            int64_t exp =
                 (int64_t)exp_in_fact(n, p)
               - (int64_t)exp_in_fact(k, p)
               - (int64_t)exp_in_fact(k + m - 1, p)
               - (int64_t)exp_in_fact(n - 2 * k - m + 1, p)
               - (int64_t)exp_in_fact(m, p)
               - (int64_t)exp_in_fact(n - k - m, p)
               + (int64_t)exp_in_fact(n - k, p);

            exp -= (int64_t)exp_in_num(n, p);     /* divide by the factor n */

            if (exp < 0) { zero = true; break; }

            for (int64_t e = 0; e < exp; ++e) term *= (seq_t)p;
        }

        if (!zero) sum += term;
    }

    return sum;
}

/* ------------------------------------------------------------
 * Convert the sequence index n to a triangle (row, column)
 * and return the corresponding term.
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = 0;
    /* find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;          /* column in the r‑th row */

    return compute_T(r, c);
}
