/*
 * A353019  Heptagonal numbers which are products of five distinct primes.
 * https://oeis.org/A353019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th heptagonal number that is a product of five
 *            distinct primes
 * Range:     u8 none, u16 n <= 2, u32 n <= 3266, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

static idx_t *primes_arr = NULL;   /* prime list */
static size_t prime_cnt  = 0;       /* number of primes */

/* initialise the prime list once */
static void init_primes(void)
{
    if (primes_arr) return;        /* already initialised */
    const idx_t PR_LIMIT = 4000000; /* sufficient for the required n */
    build_primes_upto(PR_LIMIT, &primes_arr, &prime_cnt);
}

/* factor a 64‑bit number in a square‑free way.
   returns 0 if square‑free, 1 otherwise.
   primes_arr holds the distinct prime factors (up to 32 primes). */
static int factor_squarefree(uint64_t n, int *out_cnt, int primes_out[32])
{
    int cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = (uint64_t)primes_arr[i];
        if (p * p > n) break;
        if (n % p == 0)
        {
            int mult = 0;
            while (n % p == 0)
            {
                n /= p;
                ++mult;
                if (mult > 1) return 1;   /* prime power > 1 → not square‑free */
            }
            if (cnt < 32) primes_out[cnt++] = (int)p;
            else return 1;               /* too many distinct factors (shouldn't happen) */
        }
    }
    if (n > 1)          /* remaining prime factor > sqrt(original n) */
    {
        if (cnt < 32) primes_out[cnt++] = (int)n;
        else return 1;
    }
    *out_cnt = cnt;
    return 0;           /* square‑free */
}

/* state for memoisation */
static idx_t      next_m   = 1;   /* next m to test */
static seq_t     *seq      = NULL;/* memoised sequence values */
static size_t     seq_len  = 0;   /* number of stored terms */
static size_t     seq_cap  = 0;   /* allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    init_primes();

    while (seq_len < n)
    {
        uint64_t m = (uint64_t)next_m;
        uint64_t a_part, b_part;

        if ((m & 1ULL) == 0)                 /* m even: 2 | m, use (m/2)*(5m-3) */
        {
            a_part = m / 2;
            b_part = 5 * m - 3;
        }
        else                                 /* m odd: 2 | (5m-3), use m*((5m-3)/2) */
        {
            a_part = m;
            b_part = (5 * m - 3) / 2;
        }

        int primes_a[32], primes_b[32];
        int ca, cb;

        if (factor_squarefree(a_part, &ca, primes_a)) { ++next_m; continue; }
        if (factor_squarefree(b_part, &cb, primes_b)) { ++next_m; continue; }

        /* check that no prime occurs in both parts (ensures exponent 1 overall) */
        bool bad = false;
        for (int i = 0; i < cb && !bad; ++i)
            for (int j = 0; j < ca; ++j)
                if (primes_b[i] == primes_a[j]) { bad = true; break; }

        if (bad) { ++next_m; continue; }

        if (ca + cb == 5)                     /* exactly five distinct primes */
        {
            seq_t val = (seq_t)next_m *
                        ((seq_t)5 * (seq_t)next_m - (seq_t)3) / 2;
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = seq_realloc(seq, seq_len, seq_cap);
            }
            seq[seq_len++] = val;
        }

        ++next_m;
    }

    return seq[n - 1];
}
