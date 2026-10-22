/*
 * A324592  Square array T(n, k) read by diagonals, n > 0, k > 0; for any
 *          number m > 0 with prime factorization Product_{i > 0}
 *          prime(i)^e(i), let f(m) = Sum_{i > 0} e(i) * sqrt(A005117(i)); f
 *          establishes a bijection between the positive numbers and the
 *          finite sums of square roots of squarefree numbers; let...
 * https://oeis.org/A324592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(antidiagonal_coords(n)) Compute the OEIS sequence
 *            A324592 by evaluating the commutative, associative, completely
 *            multiplicative operation T on the pair of indices obtained from
 *            the diagonal enumeration of the square array
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* ------------------------------------------------------------------ */
/* static tables, built once on first call                            */
/* ------------------------------------------------------------------ */

#define MAX_PRIME_LIST 200
#define MAX_SQF        5000
#define SQF_BOUND      50000

static uint64_t primes_small[MAX_PRIME_LIST];
static size_t   primes_small_cnt = 0;

static int prime_to_index[201] = {0};   /* prime -> its index (1‑based) */
static uint64_t sqf[MAX_SQF + 1];
static size_t   sqf_cnt         = 0;
static int      sqf_index[SQF_BOUND + 1];

static seq_t T_pp[MAX_PRIME_LIST][MAX_PRIME_LIST];
static bool  T_pp_computed[MAX_PRIME_LIST][MAX_PRIME_LIST];
static bool  tables_initialized = false;

/* ------------------------------------------------------------------ */
/* helper: integer sqrt (floor) for 64‑bit values                     */
/* ------------------------------------------------------------------ */
static idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* ------------------------------------------------------------------ */
/* build all static tables on first usage                             */
/* ------------------------------------------------------------------ */
static void init_tables(void) {
    if (tables_initialized) return;
    tables_initialized = true;

    /* sieve primes up to 300 */
    bool sieve[301];
    for (size_t i = 0; i <= 300; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= 300; ++i) if (sieve[i]) {
        for (size_t j = i * i; j <= 300; j += i) sieve[j] = false;
    }
    for (size_t i = 2; i <= 300; ++i) if (sieve[i]) {
        primes_small[primes_small_cnt++] = i;
    }

    /* map small primes to their index (1‑based) */
    for (size_t i = 0; i < primes_small_cnt; ++i) {
        if (primes_small[i] <= 200)
            prime_to_index[(int)primes_small[i]] = (int)(i + 1);
    }

    /* generate square‑free numbers up to SQF_BOUND */
    for (uint64_t val = 1; val <= SQF_BOUND && sqf_cnt < MAX_SQF; ++val) {
        uint64_t temp = val;
        bool squarefree = true;
        for (size_t i = 0; i < primes_small_cnt && primes_small[i] * primes_small[i] <= temp; ++i) {
            if (temp % primes_small[i] == 0) {
                temp /= primes_small[i];
                if (temp % primes_small[i] == 0) {
                    squarefree = false;
                    break;
                }
            }
        }
        if (squarefree) {
            sqf[++sqf_cnt] = val;
            if (val <= SQF_BOUND) sqf_index[(int)val] = (int)sqf_cnt;
        }
    }

    /* initialise computed flags */
    for (size_t i = 0; i < MAX_PRIME_LIST; ++i)
        for (size_t j = 0; j < MAX_PRIME_LIST; ++j)
            T_pp_computed[i][j] = false;
}

/* ------------------------------------------------------------------ */
/* simple prime factorisation for numbers ≤ SQF_BOUND                */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n,
                          uint64_t *pr,
                          uint8_t *exp,
                          size_t *cnt) {
    *cnt = 0;
    for (size_t i = 0; i < primes_small_cnt &&
                         primes_small[i] * primes_small[i] <= n; ++i) {
        if (n % primes_small[i] == 0) {
            uint8_t e = 0;
            do {
                n /= primes_small[i];
                ++e;
            } while (n % primes_small[i] == 0);
            pr[(*cnt)++] = primes_small[i];
            exp[(*cnt) - 1] = e;
        }
    }
    if (n > 1) {
        pr[(*cnt)++] = n;
        exp[(*cnt) - 1] = 1;
    }
}

/* ------------------------------------------------------------------ */
/* integer power for small exponents (≤256)                           */
/* ------------------------------------------------------------------ */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= base;
    return res;
}

/* ------------------------------------------------------------------ */
/* compute T(p,q) for prime arguments p,q (p≤200, q≤200)              */
/* ------------------------------------------------------------------ */
static seq_t compute_T_pp_pair(uint64_t p, uint64_t q) {
    init_tables();

    int idxp = prime_to_index[(int)p];
    int idxq = prime_to_index[(int)q];

    if (T_pp_computed[idxp][idxq]) return T_pp[idxp][idxq];

    uint64_t sp = sqf[idxp];
    uint64_t sq = sqf[idxq];

    uint64_t pr_p[10], pr_q[10];
    uint8_t  ex_p[10], ex_q[10];
    size_t   cnt_p, cnt_q;
    factor_uint64(sp, pr_p, ex_p, &cnt_p);
    factor_uint64(sq, pr_q, ex_q, &cnt_q);

    uint64_t prod_common = 1;
    uint64_t prod_sym    = 1;

    for (size_t i = 0; i < cnt_p; ++i) {
        bool found = false;
        for (size_t j = 0; j < cnt_q; ++j) {
            if (pr_p[i] == pr_q[j]) {
                found = true;
                prod_common *= pr_p[i];
                break;
            }
        }
        if (!found) prod_sym *= pr_p[i];
    }
    for (size_t i = 0; i < cnt_q; ++i) {
        bool found = false;
        for (size_t j = 0; j < cnt_p; ++j) {
            if (pr_q[i] == pr_p[j]) {
                found = true;
                break;
            }
        }
        if (!found) prod_sym *= pr_q[i];
    }

    int sym_idx = sqf_index[(int)prod_sym];
    seq_t base_prime = (seq_t)get_prime((idx_t)sym_idx);
    seq_t val = pow_seq(base_prime, prod_common);

    T_pp[idxp][idxq] = val;
    T_pp_computed[idxp][idxq] = true;
    T_pp[idxq][idxp] = val;
    T_pp_computed[idxq][idxp] = true;

    return val;
}

/* ------------------------------------------------------------------ */
/* evaluate T(n,k) for general positive integers n,k ≤ 150            */
/* ------------------------------------------------------------------ */
static seq_t T_value(idx_t n, idx_t k) {
    init_tables();

    if (n == 1 || k == 1) return 1;

    uint64_t n64 = (uint64_t)n;
    uint64_t k64 = (uint64_t)k;

    uint64_t pr_n[10], pr_k[10];
    uint8_t  ex_n[10], ex_k[10];
    size_t   cnt_n, cnt_k;
    factor_uint64(n64, pr_n, ex_n, &cnt_n);
    factor_uint64(k64, pr_k, ex_k, &cnt_k);

    seq_t res = 1;
    for (size_t i = 0; i < cnt_n; ++i) {
        for (size_t j = 0; j < cnt_k; ++j) {
            seq_t base = compute_T_pp_pair(pr_n[i], pr_k[j]);
            idx_t power = (idx_t)ex_n[i] * (idx_t)ex_k[j];
            seq_t mult = pow_seq(base, power);
            res *= mult;
        }
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* convert the n‑th index to its diagonal coordinates (i,j)           */
/* ------------------------------------------------------------------ */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j) {
    /* find diagonal k such that t_{k-1} < n ≤ t_k, t_k = k(k+1)/2 */
    idx_t k = (isqrt_u64(8 * n + 1) + 1) / 2;
    while (k * (k + 1) / 2 < n) ++k;
    idx_t t_prev = (k - 1) * k / 2;
    idx_t p = n - t_prev;           /* position inside diagonal, 1‑based */
    *i = p;
    *j = k - p + 1;
}

/* ------------------------------------------------------------------ */
/* public entry point: compute a(n)                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return T_value(i, j);
}
