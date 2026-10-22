/*
 * A336013  Three-column table read by rows giving triples of integers with x
 *          > 0, y > 1 and z > 0 such that y^2 - y - x*z = 0, sorted by y then
 *          by x.
 * https://oeis.org/A336013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 428, u16 n <= 2898, u32 n <= 2898, u64 n <= 2898,
 *            u128 n <= 2898, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A336013: successive triples (x,y,z) with y>1, x>0, z>0
 * satisfying y^2 - y - x*z = 0, sorted by y then by x,
 * then flattened row‑wise. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Compare two uint64_t values for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Recursively generate all divisors from prime factor lists. */
static void gen_divs_rec(const uint64_t *primes, const uint8_t *exps,
                         size_t cnt, size_t idx, uint64_t cur,
                         uint64_t *divs, size_t *div_cnt)
{
    if (idx == cnt) {
        divs[(*div_cnt)++] = cur;
        return;
    }
    uint64_t p_pow = 1;
    for (uint8_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(primes, exps, cnt, idx + 1, cur * p_pow,
                     divs, div_cnt);
        p_pow *= primes[idx];
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used; n>=1 by spec */

    idx_t sum_terms = 0;              /* terms covered so far */
    for (idx_t y = 2; ; ++y) {
        uint64_t y_val = (uint64_t)y;
        /* compute D = y*(y-1) safely */
        __int128 tmp = (__int128)y_val * (y_val - 1);
        uint64_t D = (uint64_t)tmp;

        /* factorize D */
        uint64_t primes[16];
        uint8_t exps[16];
        size_t prime_cnt = 0;
        uint64_t temp = D;
        for (uint64_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                uint8_t e = 0;
                while (temp % p == 0) { temp /= p; ++e; }
                primes[prime_cnt] = p;
                exps[prime_cnt] = e;
                ++prime_cnt;
            }
        }
        if (temp > 1) {
            primes[prime_cnt] = temp;
            exps[prime_cnt] = 1;
            ++prime_cnt;
        }

        /* number of divisors */
        size_t cnt_div = 1;
        for (size_t i = 0; i < prime_cnt; ++i)
            cnt_div *= (size_t)(exps[i] + 1);

        /* generate all divisors */
        uint64_t divs[1024];
        size_t div_cnt = 0;
        gen_divs_rec(primes, exps, prime_cnt, 0, 1, divs, &div_cnt);

        /* sort divisors */
        qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

        /* block term count */
        idx_t block_terms = (idx_t)cnt_div * 3;
        if (sum_terms + block_terms < n) {
            sum_terms += block_terms;
            continue;
        }

        /* we are in this block */
        idx_t offset = n - sum_terms - 1;          /* 0‑based */
        size_t triple_idx = (size_t)(offset / 3);   /* which divisor */
        size_t pos_in_triple = (size_t)(offset % 3);/* 0:x,1:y,2:z */

        uint64_t x = divs[triple_idx];
        uint64_t z = D / x;

        if (pos_in_triple == 0) return (seq_t)x;
        if (pos_in_triple == 1) return (seq_t)y;
        return (seq_t)z;
    }
}
