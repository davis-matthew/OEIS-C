/*
 * A384469  a(n) is the number of triples 1 <= A, B, C <= n such that the
 *          discriminant D = B^2 - 4*A*C of the polynomial A*x^2 + B*x + C is
 *          0.
 * https://oeis.org/A384469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (A, B, C) with 1 ≤ A, B, C ≤ n such
 *            that B^2 - 4*A*C = 0
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Smallest‑prime‑factor table (sieve)                                 */
/* ------------------------------------------------------------------ */

static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Build spf up to limit (inclusive) */
static void build_spf(idx_t limit) {
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) return;                 /* out of memory – give up */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Recursively count valid divisors of k^2 that satisfy bounds       */
/* ------------------------------------------------------------------ */

static void count_divisors(const idx_t *primes, const int *exp,
                           int sz, int pos, idx_t cur,
                           idx_t k2, idx_t n, seq_t *cnt) {
    if (pos == sz) {
        if (cur <= n && k2 / cur <= n) ++(*cnt);
        return;
    }
    idx_t p = primes[pos];
    int e = exp[pos];
    idx_t val = cur;
    for (int i = 0; i <= e; ++i) {
        if (val > n) break;          /* no need to examine larger divisors */
        count_divisors(primes, exp, sz, pos + 1, val, k2, n, cnt);
        val *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n < 2) return 0;

    idx_t kmax = n / 2;
    build_spf(kmax);

    seq_t total = 0;

    /* Temporary storage for factorisation data */
    idx_t primes[64];
    int exp_k[64];
    int exp_k2[64];

    for (idx_t k = 1; k <= kmax; ++k) {
        /* Factor k */
        idx_t tmp = k;
        int sz = 0;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++c;
            }
            primes[sz] = p;
            exp_k[sz] = c;
            ++sz;
        }
        for (int i = 0; i < sz; ++i) exp_k2[i] = exp_k[i] * 2;

        idx_t k2 = k * k;
        count_divisors(primes, exp_k2, sz, 0, 1, k2, n, &total);
    }

    return total;
}
