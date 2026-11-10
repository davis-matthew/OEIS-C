/*
 * A391343  Odd semiprimes k = p*q such that either k = A048720(A065621(p),x)
 *          or k = A048720(A065621(q),x) for some x.
 * https://oeis.org/A391343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd semiprime k=p*q such that A065621(p) or
 *            A065621(q) divides k as a GF(2)[x] polynomial (k =
 *            A048720(A065621(p),x) for some x); found by search with trial
 *            division
 * Range:     u8 n <= 12, u16 n <= 1073, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9895 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int gf_bits(seq_t x)
{
    int b = 0;
    while (x > 0) { b++; x >>= 1; }
    return b;
}

/* A065621 */
static seq_t A065621_a(seq_t n)
{
    seq_t low = n & (seq_t)(0 - n);
    return n ^ ((n - low) << 1);
}

/* does polynomial r divide polynomial k over GF(2)? */
static bool gf_divides(seq_t k, seq_t r)
{
    int lk = gf_bits(k), lr = gf_bits(r);
    if (lr == 0 || lr > lk) return false;
    for (int i = lk - 1; i >= lr - 1; i--) {
        if (((k >> i) & 1) != 0)
            k ^= (r << (i - (lr - 1)));
    }
    return k == 0;
}

static bool pq_check(seq_t k, seq_t p)
{
    int lk = gf_bits(k);
    int lp = gf_bits(p);
    if (lp + 1 > lk) return false; /* reversed value too large to divide k */
    seq_t r = A065621_a(p);
    return gf_divides(k, r);
}

static bool small_prime(seq_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (seq_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return false;
    return true;
}

static bool qualifies(seq_t k)
{
    seq_t p = 0;
    for (seq_t d = 3; d * d <= k; d += 2) {
        if (k % d == 0) { p = d; break; }
    }
    if (p == 0) return false;
    seq_t q = k / p;
    if (!small_prime(q)) return false;
    return pq_check(k, p) || pq_check(k, q);
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0;
    static seq_t last_k = 7;
    if (n < last_n || last_n == 0) {
        last_n = 0;
        last_k = 7;
    }
    idx_t cnt = last_n;
    seq_t k = last_k;
    while (cnt < n) {
        k += 2;
        if (qualifies(k)) cnt++;
    }
    last_n = cnt;
    last_k = k;
    return k;
}
