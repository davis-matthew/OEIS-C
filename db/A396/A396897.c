/*
 * A396897  a(n) = n divided by the largest ratio of consecutive divisors of
 *          n.
 * https://oeis.org/A396897
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / max_i (d_{i+1}/d_i), where d_1..d_t are the divisors
 *            of n in increasing order. The function returns a(2) = 1, a(3) =
 *            1, a(4) = 2, … as defined in the OEIS entry
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root (floor) for 64‑bit unsigned values. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Prime factorisation of n (unsigned 64‑bit).  pr[i] and ex[i] hold
   the i‑th prime factor and its exponent.  cnt receives the number
   of distinct factors.  For n <= 10000 at most 6 primes are needed. */
static void factor_uint64(idx_t n, idx_t pr[32], idx_t ex[32], size_t *cnt)
{
    size_t c = 0;
    if (n < 2) {
        *cnt = 0;
        return;
    }
    while ((n & 1ULL) == 0) {               /* factor 2 */
        ex[c] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ex[c]++;
        }
        pr[c] = 2;
        c++;
    }
    idx_t p = 3;
    idx_t limit = isqrt_u64(n);
    while (p <= limit) {
        if (n % p == 0) {
            ex[c] = 0;
            while (n % p == 0) {
                n /= p;
                ex[c]++;
            }
            pr[c] = p;
            c++;
            limit = isqrt_u64(n);
        }
        p += 2;
    }
    if (n > 1) {                            /* remaining prime factor */
        pr[c] = n;
        ex[c] = 1;
        c++;
    }
    *cnt = c;
}

/* Generate all divisors of n from its prime factorisation.  The result
   is stored in divs[0..len-1] in arbitrary order.  The function
   returns the number of divisors.  For n <= 10000 the maximum number
   of divisors is < 200, so a buffer of 4096 suffices. */
static size_t gen_divisors(idx_t n, const idx_t pr[32], const idx_t ex[32], size_t cnt, idx_t *divs)
{
    size_t len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = pr[i];
        idx_t p_pow = p;
        size_t old_len = len;
        for (idx_t k = 1; k <= ex[i]; ++k) {
            for (size_t j = 0; j < old_len; ++j) {
                divs[len++] = divs[j] * p_pow;
            }
            p_pow *= p;
        }
    }
    return len;
}

/* Comparator for qsort on idx_t values. */
static int cmp_idx(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* behaviour below offset is irrelevant */

    /* 1. factorise n */
    idx_t primes[32], exps[32];
    size_t pf_cnt;
    factor_uint64(n, primes, exps, &pf_cnt);

    /* 2. generate all divisors of n */
    idx_t divs[4096];
    size_t len = gen_divisors(n, primes, exps, pf_cnt, divs);

    /* 3. sort divisors */
    qsort(divs, len, sizeof(idx_t), cmp_idx);

    /* 4. find the maximal consecutive ratio d_{i+1}/d_i
       by comparing cross‑products to avoid floating point. */
    idx_t best_num = divs[1];               /* d_{i+1} */
    idx_t best_den = divs[0];               /* d_i */
    for (size_t i = 0; i + 1 < len; ++i) {
        idx_t x = divs[i];
        idx_t y = divs[i + 1];
        /* compare y/x > best_num/best_den  <=>  y*best_den > best_num*x */
        if ( (unsigned __int128)y * best_den > (unsigned __int128)best_num * x ) {
            best_num = y;
            best_den = x;
        }
    }

    /* 5. compute a(n) = (n / best_num) * best_den */
    seq_t part = (seq_t)(n / best_num);
    seq_t res  = part * (seq_t)best_den;
    return res;
}
