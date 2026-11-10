/*
 * A265394  Positions of records in A265388.
 * https://oeis.org/A265394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n-th strict record in A265388, where
 *            A265388(m) = 2^[m power of 2, m>1] * prod of odd primes p with
 *            2m = p^i + p^j (i<=j); computed by sequential scan with a
 *            smallest-prime-factor sieve, comparing values via sums of logs
 *            of their prime sets
 * Range:     u8 n <= 16, u16 n <= 843, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9558 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static uint32_t *A265394_spf = NULL;
static uint64_t A265394_lim = 0;

static void A265394_sieve(uint64_t limit)
{
    if (A265394_spf && A265394_lim >= limit)
        return;
    if (A265394_spf)
        free(A265394_spf);
    uint64_t L = limit * 2 + 16;
    A265394_spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    for (uint64_t i = 2; i <= L; i++) {
        if (A265394_spf[i] == 0) {
            for (uint64_t j = i; j <= L; j += i)
                if (A265394_spf[j] == 0)
                    A265394_spf[j] = (uint32_t)i;
        }
    }
    A265394_lim = L;
}

/* collect primes of A265388(m) into ps, return count (m>=2) */
static int A265394_primes(uint64_t m, uint64_t *ps)
{
    int c = 0;
    uint64_t two_m = 2 * m;
    if ((m & (m - 1)) == 0)
        ps[c++] = 2;
    uint64_t x = two_m - 1;
    if (x > 1) {
        uint64_t p = A265394_spf[x];
        while (x % p == 0)
            x /= p;
        if (x == 1)
            ps[c++] = p;
    }
    uint64_t y = m;
    while (y % 2 == 0)
        y /= 2;
    while (y > 1) {
        uint64_t p = A265394_spf[y];
        while (y % p == 0)
            y /= p;
        bool found = false;
        uint64_t pi = p;
        while (pi < two_m && !found) {
            uint64_t r = two_m - pi;
            if (r % pi == 0) {
                r /= pi;
                while (r % p == 0)
                    r /= p;
                if (r == 1)
                    found = true;
            }
            if (pi > two_m / p)
                break;
            pi *= p;
        }
        if (found)
            ps[c++] = p;
    }
    return c;
}

seq_t a(idx_t n)
{
    static uint64_t *rec = NULL;
    static size_t rec_cnt = 0, rec_cap = 0;
    static uint64_t cur_m = 1;
    static double best_log = -1.0;
    static uint64_t best_ps[64];
    static int best_cnt = 0;

    if (rec == NULL) {
        rec_cap = 1024;
        rec = (uint64_t *)malloc(rec_cap * sizeof(uint64_t));
        rec[rec_cnt++] = 1;   /* m=1, value 0 */
    }

    while (rec_cnt < (size_t)n) {
        cur_m++;
        A265394_sieve(2 * cur_m + 2);
        uint64_t ps[64];
        int c = A265394_primes(cur_m, ps);
        double lg = 0.0;
        for (int i = 0; i < c; i++)
            lg += log((double)ps[i]);
        bool rcd = false;
        if (lg > best_log + 1e-9) {
            rcd = true;
        } else if (lg > best_log - 1e-9) {
            /* near tie: equal sets are not a strict record */
            bool same = (c == best_cnt);
            if (same) {
                for (int i = 0; i < c && same; i++) {
                    bool f = false;
                    for (int j = 0; j < best_cnt; j++)
                        if (best_ps[j] == ps[i]) { f = true; break; }
                    if (!f) same = false;
                }
            }
            if (!same && lg > best_log)
                rcd = true;
        }
        if (rcd) {
            best_log = lg;
            best_cnt = c;
            for (int i = 0; i < c; i++)
                best_ps[i] = ps[i];
            if (rec_cnt == rec_cap) {
                rec_cap *= 2;
                rec = (uint64_t *)realloc(rec, rec_cap * sizeof(uint64_t));
            }
            rec[rec_cnt++] = cur_m;
        }
    }
    return (seq_t)rec[n - 1];
}
