/*
 * A366447  Number of commutative discrete aggregation functions defined on
 *          the finite chain L_n={0,1,...,n-1,n}.
 * https://oeis.org/A366447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n+1}((2i+n-1)/(2i-1) *
 *            Product_{j=i+1..n+1}((i+j+n-1)/(i+j-1))) - 2 *
 *            Product_{i=1..n+1}((2i+n-2)/(2i-1) *
 *            Product_{j=i+1..n+1}((i+j+n-2)/(i+j-1))) +
 *            Product_{i=1..n+1}((2i+n-3)/(2i-1) *
 *            Product_{j=i+1..n+1}((i+j+n-3)/(i+j-1)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------  helper utilities  ------------ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/gcd.h"

/* Count unordered pairs (i,j), 1 ≤ i < j ≤ N, with i+j = s */
static inline idx_t pair_count(idx_t s, idx_t N)
{
    if (s < 3 || s > 2 * N) return 0;
    idx_t low = (s <= N + 1) ? 1 : s - N;
    if (s & 1) {                /* s odd */
        idx_t high = (s - 1) / 2;
        return (high >= low) ? (high - low + 1) : 0;
    } else {                     /* s even */
        idx_t high = s / 2 - 1;  /* i must be strictly less than s/2 */
        return (high >= low) ? (high - low + 1) : 0;
    }
}

/* Smallest prime factor table (dynamic) */
static int32_t *spf = NULL;
static idx_t spf_size = 0;

static void ensure_spf(idx_t limit)
{
    if (limit <= spf_size) return;
    free(spf);
    spf = (int32_t *)malloc((limit + 1) * sizeof(int32_t));
    if (!spf) return;
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (int32_t)i;
    if (limit >= 2) for (idx_t i = 4; i <= limit; i += 2) spf[i] = 2;
    for (idx_t i = 3; (idx_t)(i * i) <= limit; i += 2) {
        if (spf[i] == (int32_t)i) {
            for (idx_t j = i * i; j <= limit; j += i * 2) {
                if (spf[j] == (int32_t)j) spf[j] = (int32_t)i;
            }
        }
    }
    spf_size = limit;
}

static void add_factor(int32_t *exp, idx_t x, idx_t cnt, int sign)
{
    while (x > 1) {
        int p = spf[x];
        int c = 0;
        while (x % p == 0) {
            x /= p;
            ++c;
        }
        exp[p] += sign * (int32_t)(c * cnt);
    }
}

/* Compute the product described in the formula for a given offset.
   Returns 0 immediately if any numerator becomes zero. */
static seq_t product_value(idx_t n, int offset)
{
    idx_t N = n + 1;
    idx_t limit = 3 * n + 1;   /* maximum integer appearing in factors */
    ensure_spf(limit);

    /* Exp array for prime exponents */
    int32_t *exp = (int32_t *)calloc(limit + 1, sizeof(int32_t));
    if (!exp) return 0;

    /* Handle single-factor part: i = 1..N */
    for (idx_t i = 1; i <= N; ++i) {
        idx_t num = 2 * i + n + (idx_t)offset;
        if (num == 0) { free(exp); return 0; }      /* zero numerator → product 0 */
        idx_t den = 2 * i - 1;
        add_factor(exp, num, 1, +1);
        add_factor(exp, den, 1, -1);
    }

    /* Handle double-factor part: pairs (i,j) with i<j */
    for (idx_t s = 3; s <= 2 * N; ++s) {
        idx_t cnt = pair_count(s, N);
        if (cnt == 0) continue;
        idx_t num = s + n + (idx_t)offset;
        if (num == 0) { free(exp); return 0; }      /* zero numerator → product 0 */
        idx_t den = s - 1;
        add_factor(exp, num, cnt, +1);
        add_factor(exp, den, cnt, -1);
    }

    /* Build the integer from prime exponents */
    seq_t result = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        int32_t e = exp[p];
        if (e > 0) {
            seq_t pow_p = seq_pow((seq_t)p, (idx_t)e);
            result = seq_mul(result, pow_p);
        }
    }
    free(exp);
    return result;
}

/* -----------------  sequence function  ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not defined; harmless placeholder */

    seq_t p1 = product_value(n, -1);  /* offset = -1 */
    seq_t p2 = product_value(n, -2);  /* offset = -2 */
    seq_t p3 = product_value(n, -3);  /* offset = -3 */

    seq_t two_p2 = p2 + p2;          /* 2 * P(-2) */
    seq_t temp = p1 + p3;
    seq_t result = temp - two_p2;
    return result;
}
