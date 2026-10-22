/*
 * A327514  Quotient of n over the maximum divisor of n that is 1, 2, or a
 *          nonprime number whose prime indices are pairwise coprime.
 * https://oeis.org/A327514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / dmax , where dmax is the maximum divisor of n that is
 *            1, 2, or a nonprime number whose prime indices are pairwise
 *            coprime
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities */
#include "../utils/factor_u64.h"

/* --- helpers ----------------------------------------------------------- */

/* gcd for 32‑bit integers */
static inline uint32_t gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- sieve to obtain prime indices ------------------------------------ */
static uint8_t *sieve_is_comp = NULL;          /* 0 = prime, 1 = composite */
static uint64_t *sieve_primes = NULL;          /* list of primes in order */
static size_t    sieve_prime_cnt  = 0;
static uint64_t  sieve_max         = 0;

/* extend the sieve up to limit (inclusive) */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_max)
        return;

    /* free old data */
    free(sieve_is_comp);
    free(sieve_primes);
    sieve_is_comp = NULL;
    sieve_primes = NULL;
    sieve_prime_cnt = 0;
    sieve_max = 0;

    size_t lim = (size_t)limit + 1;                     /* array size 0..limit */
    sieve_is_comp = (uint8_t*)calloc(lim, 1);
    if (!sieve_is_comp)
        return;                                       /* allocation failed */

    /* classic sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve_is_comp[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                sieve_is_comp[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve_is_comp[i])
            ++cnt;

    sieve_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!sieve_primes)
        return;                                       /* allocation failed */

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve_is_comp[i])
            sieve_primes[idx++] = i;

    sieve_prime_cnt = cnt;
    sieve_max       = limit;
}

/* return the 1‑based index of a prime p (assumes p is prime) */
static uint32_t prime_index_of(uint64_t p)
{
    if (p < 2)
        return 0;                                     /* not a prime */

    ensure_sieve(p);

    /* binary search in sieve_primes */
    size_t lo = 0, hi = sieve_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t v = sieve_primes[mid];
        if (v < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < sieve_prime_cnt && sieve_primes[lo] == p)
        return (uint32_t)(lo + 1);                    /* 1‑based index */
    return 0;                                         /* should not happen */
}

/* --- divisor enumeration -------------------------------------------- */
static void rec_div(size_t pos, seq_t cur, bool *used, uint32_t *sel_exp,
                    uint64_t *pr, uint64_t *ex,
                    uint32_t *idx_arr, size_t cnt, seq_t *best)
{
    if (pos == cnt) {
        seq_t d = cur;

        /* 1 and 2 are always allowed */
        if (d == 1 || d == (seq_t)2) {
            if (d > *best)
                *best = d;
            return;
        }

        /* gather used primes and their exponents */
        size_t used_cnt = 0;
        size_t used_idx[64];
        uint32_t used_exp[64];
        for (size_t k = 0; k < cnt; ++k) {
            if (used[k]) {
                used_idx[used_cnt] = idx_arr[k];
                used_exp[used_cnt] = sel_exp[k];
                ++used_cnt;
            }
        }

        /* reject a prime divisor > 2 */
        if (used_cnt == 1 && used_exp[0] == 1 && used_idx[0] != 1)
            return;

        /* pairwise coprime indices check */
        for (size_t a = 0; a < used_cnt && *best < d; ++a) {
            for (size_t b = a + 1; b < used_cnt && *best < d; ++b) {
                if (gcd_uint32(used_idx[a], used_idx[b]) != 1)
                    return;
            }
        }

        if (d > *best)
            *best = d;
        return;
    }

    /* exponent 0 */
    used[pos] = false;
    sel_exp[pos] = 0;
    rec_div(pos + 1, cur, used, sel_exp, pr, ex, idx_arr, cnt, best);

    /* exponents 1 .. ex[pos] */
    seq_t prod = cur;
    for (uint64_t e = 1; e <= ex[pos]; ++e) {
        prod *= (seq_t)pr[pos];
        if (idx_arr[pos] > 1 && e > 1)          /* repeated index >1 not allowed */
            break;
        used[pos] = true;
        sel_exp[pos] = (uint32_t)e;
        rec_div(pos + 1, prod, used, sel_exp, pr, ex, idx_arr, cnt, best);
    }
}

/* --- public sequence function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                   /* below offset */

    /* factor n */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* prime indices for the factors */
    uint32_t idx_arr[64];
    for (size_t i = 0; i < cnt; ++i)
        idx_arr[i] = prime_index_of(pr[i]);

    bool used[64]   = {0};
    uint32_t sel_exp[64] = {0};

    seq_t best = 1;                                 /* at least divisor 1 */
    rec_div(0, (seq_t)1, used, sel_exp, pr, ex, idx_arr, cnt, &best);

    seq_t result = (seq_t)n / best;
    return result;
}
