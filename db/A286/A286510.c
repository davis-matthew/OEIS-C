/*
 * A286510  Number of primitive roots g mod prime(n) for which there is no
 *          solution to g^x == x (mod p) with 2 <= x <= prime(n)-2.
 * https://oeis.org/A286510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008330(n) - A174407(n) for n >= 2; that is, number of
 *            primitive roots modulo the n-th prime which do NOT satisfy g^x ≡
 *            x (mod p) for any integer x with 2 ≤ x ≤ p-2
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factorize, gcd_int, get_prime, mod_inv_int64, mod_mul, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary functions */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t, uint64_t, seq_t); */
#include "../utils/mod_mul.h"        /* uint64_t mod_mul(uint64_t, uint64_t, uint64_t); */
#include "../utils/gcd_int.h"        /* int gcd_int(int, int); */
#include "../utils/factorize.h"      /* uint64_t factorize(uint64_t, uint64_t *, int *); */
#include "../utils/mod_inv_int64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial primes */
    if (n == 1) return (seq_t)1;          /* prime 2  */
    if (n == 2) return (seq_t)1;          /* prime 3  */

    /* nth prime */
    uint32_t p_u32 = get_prime(n);
    seq_t p = (seq_t)p_u32;               /* fits in seq_t for all builtins */
    idx_t m = (idx_t)p_u32 - 1;           /* p-1 fits in idx_t */

    /* --------- find a fixed primitive root g0 --------- */
    /* factor m */
    uint64_t primes[32];
    int exps[32];
    uint64_t cnt = factorize((uint64_t)m, primes, exps);

    uint32_t g0 = 2;
    for (; g0 < p_u32; ++g0) {
        bool ok = true;
        for (uint64_t i = 0; i < cnt; ++i) {
            seq_t exp = mod_pow((seq_t)g0, (uint64_t)m / primes[i], p);
            if (exp == 1) { ok = false; break; }
        }
        if (ok) break;
    }

    /* --------- precompute discrete log base g0 (log_tbl[ x ]) --------- */
    /* log_tbl maps an element 1..p-1 to its exponent modulo (p-1) */
    idx_t *log_tbl = (idx_t *)malloc((size_t)p_u32 * sizeof(idx_t));
    if (!log_tbl) return (seq_t)0;  /* allocation failed */

    uint64_t val = 1;
    log_tbl[1] = 0;
    for (idx_t e = 1; e < m; ++e) {
        val = mod_mul(val, g0, p_u32);        /* compute g0^e mod p */
        log_tbl[val] = e;
    }

    /* --------- solution flag array for exponent k (0 .. m-1) --------- */
    /* solution_flag[k] == true  -> primitive root g0^k has a solution */
    bool *solution_flag = (bool *)calloc((size_t)m, sizeof(bool));
    if (!solution_flag) { free(log_tbl); return (seq_t)0; }

    /* --------- process each x (2 .. p-2) --------- */
    for (idx_t x = 2; x <= m; ++x) {              /* x ranges 2 .. p-2 */
        idx_t s = log_tbl[x];                     /* discrete log of x */
        idx_t e = x;
        int d = gcd_int((int)e, (int)m);          /* gcd(x, p-1) */
        if (s % d != 0) continue;

        idx_t x_div = e / d;
        idx_t s_div = s / d;
        idx_t mod_div = m / d;

        int64_t inv_x = mod_inv_int64((int64_t)x_div, (int64_t)mod_div);
        int64_t k0 = (s_div * inv_x) % (int64_t)mod_div;
        if (k0 < 0) k0 += mod_div;

        for (int t = 0; t < d; ++t) {
            idx_t k = (idx_t)k0 + t * mod_div;   /* 0 <= k < m */
            solution_flag[k] = true;
        }
    }

    /* --------- count primitive roots with no solution --------- */
    idx_t ans = 0;
    for (idx_t k = 0; k < m; ++k) {
        if (!solution_flag[k] && gcd_int((int)k, (int)m) == 1)
            ++ans;
    }

    /* clean up */
    free(log_tbl);
    free(solution_flag);

    return (seq_t)ans;
}
