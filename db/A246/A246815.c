/*
 * A246815  Expansion of phi(-x) * psi(-x^2)^2 in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A246815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in φ(-x) * ψ(-x²)², where φ and ψ are
 *            Ramanujan theta functions
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* In terms of explicit coefficients:  
   let s_k = (-1)^{k(k+1)/2} and t_k = k(k+1).  
   φ(-x) = 1 + Σ_{k≥1} 2*(-1)^k · x^{k²}.  
   ψ(-x²) = Σ_{k≥0} s_k · x^{t_k}.  
   Then a(n) = Σ_{r² ≤ n} c_r · b_{n−r²},  
   where c_0 = 1,  c_r = 2*(-1)^r (r≥1),  
   and b_m = Σ_{i,j: t_i+t_j = m} s_i·s_j.  */

#include <stdlib.h>

/* Static cache of computed values. */
static seq_t *a_cache = NULL;   /* a_cache[0..len-1] holds a(0)…a(len-1) */
static size_t a_len   = 0;

/* Recompute the cache up to N (inclusive).  N is a size_t. */
static void recompute_cache(size_t N)
{
    /* --- 1. Generate all t_k ≤ N and their signs s_k. --- */
    size_t cnt = 0;
    for (size_t i = 0;; ++i) {
        size_t t = i * (i + 1);
        if (t > N)
            break;
        ++cnt;
    }

    size_t *t_arr = (size_t *)calloc(cnt, sizeof(size_t));
    int8_t *s_arr = (int8_t *)calloc(cnt, sizeof(int8_t));

    for (size_t i = 0;; ++i) {
        size_t t = i * (i + 1);
        if (t > N)
            break;
        t_arr[i] = t;
        /* s_k = (-1)^{k(k+1)/2}  → pattern +, -, -, + for k ≡ 0,1,2,3 (mod 4). */
        s_arr[i] = ((i % 4 == 0 || i % 4 == 3) ? 1 : -1);
    }

    /* --- 2. Compute ψ(-x²)² coefficients up to N. --- */
    seq_t *b = (seq_t *)calloc(N + 1, sizeof(seq_t));

    for (size_t a = 0; a < cnt; ++a) {
        size_t t_a = t_arr[a];
        seq_t s_a = (seq_t)s_arr[a];
        for (size_t b_idx = 0; b_idx < cnt; ++b_idx) {
            size_t t_b = t_arr[b_idx];
            seq_t s_b = (seq_t)s_arr[b_idx];
            size_t sum = t_a + t_b;
            if (sum > N)
                continue;
            b[sum] += s_a * s_b;   /* s_a * s_b is ±1 */
        }
    }

    /* --- 3. Compute a(n) for 0..N. --- */
    seq_t *new_cache = (seq_t *)calloc(N + 1, sizeof(seq_t));

    for (size_t n = 0; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t r = 0; ; ++r) {
            size_t r2 = r * r;
            if (r2 > n)
                break;

            seq_t c_r;
            if (r == 0)
                c_r = 1;                     /* from φ(-x) constant term      */
            else
                c_r = ((r & 1) ? -2 : 2);    /* 2*(-1)^r  for r≥1              */

            sum += c_r * b[n - r2];
        }
        new_cache[n] = sum;
    }

    /* --- 4. Replace the old cache. --- */
    free(a_cache);
    a_cache = new_cache;
    a_len   = N + 1;

    /* --- 5. Clean temporary arrays. --- */
    free(t_arr);
    free(s_arr);
    free(b);
}

/* Public function: return a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;                /* idx_t is unsigned 64‑bit */
    if (n >= a_len)                     /* need to extend cache */
        recompute_cache(N);
    return a_cache[(size_t)n];
}
