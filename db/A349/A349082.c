/*
 * A349082  The number of two-term Egyptian fractions of rational numbers,
 *          x/y, 0 < x/y < 1, ordered as below. The sequence is the number of
 *          (p,q) pairs such that x/y = 1/p + 1/q where p and q are integers
 *          with p < q.
 * https://oeis.org/A349082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 990, u16 n <= 990, u32 n <= 990, u64 n <= 990,
 *            u128 n <= 990, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A349082: for the n‑th rational x/y in the ordered triangle count the number
 * of unordered pairs (p,q) with p<q such that x/y = 1/p + 1/q.
 * The algorithm factors the reduced denominator, enumerates all
 * divisors of its square, and counts the valid solutions.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_seq.h"

/* Small integer gcd, works for any seq_t */

/* Integer square root via binary search (safe for 64‑bit values) */
static idx_t floor_sqrt_idx(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* Trial‑division factorisation (small numbers only) */
static void factorise_uint64(uint64_t n, uint32_t *primes, uint8_t *exps,
                             size_t *cnt) {
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[*cnt] = (uint32_t)p;
            exps[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[*cnt] = (uint32_t)n;
        exps[*cnt] = 1;
        ++(*cnt);
    }
}

/* Recursive enumeration of divisors of b^2 */
static void count_divisors(const uint32_t *primes, const uint8_t *exps2,
                           size_t idx, size_t tot, uint64_t cur,
                           uint64_t b, uint64_t a, seq_t *res) {
    if (idx == tot) {
        uint64_t d = cur;
        uint64_t e = (b * b) / d;
        uint64_t p_num = d + b;
        if (p_num % a != 0) return;
        uint64_t p = p_num / a;
        uint64_t q_num = e + b;
        if (q_num % a != 0) return;
        uint64_t q = q_num / a;
        if (p < q && p > 0 && q > 0) ++(*res);
        return;
    }
    uint64_t cur_local = cur;
    for (uint8_t e = 0; e <= exps2[idx]; ++e) {
        count_divisors(primes, exps2, idx + 1, tot, cur_local,
                       b, a, res);
        cur_local *= (uint64_t)primes[idx];
    }
}

/* Count solutions for reduced fraction a/b */
static seq_t solutions_for_reduced(seq_t a_seq, seq_t b_seq) {
    uint64_t a = (uint64_t)a_seq;
    uint64_t b = (uint64_t)b_seq;
    if (b == 0) return 0;
    uint32_t primes[64];
    uint8_t exps[64];
    size_t cnt = 0;
    factorise_uint64(b, primes, exps, &cnt);

    /* exponents for b^2 */
    uint8_t exps2[64];
    for (size_t i = 0; i < cnt; ++i) exps2[i] = exps[i] * 2;

    seq_t res = 0;
    count_divisors(primes, exps2, 0, cnt, 1ULL, b, a, &res);
    return res;
}

/* Main function required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for offset 1, but safe */

    /* Find denominator y: smallest y with y*(y-1)/2 >= n */
    idx_t D = 1 + 8 * n;
    idx_t sqrtD = floor_sqrt_idx(D);
    idx_t y = (1 + sqrtD) >> 1;
    if (y * (y - 1) / 2 < n) ++y;

    /* Position within this row gives numerator x */
    idx_t terms_before = (y - 1) * (y - 2) / 2;
    idx_t pos = n - terms_before;  /* 1‑based position in the row */
    idx_t x = pos;

    /* Reduce the fraction x/y */
    seq_t g = gcd_seq((seq_t)x, (seq_t)y);
    seq_t a_red = (seq_t)x / g;
    seq_t b_red = (seq_t)y / g;

    /* Count Egyptian fraction representations */
    return solutions_for_reduced(a_red, b_red);
}
