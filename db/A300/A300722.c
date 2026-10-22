/*
 * A300722  Difference between A060681 (largest difference between consecutive
 *          divisors of n as ordered by size) and its Möbius transform.
 * https://oeis.org/A300722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A060681(n) – Σ_{d|n} μ(d) · A060681(n/d)
 * Range:     u8 n <= 513, u16 n <= 8173, u32 n <= 8173, u64 n <= 8173,
 *            u128 n <= 8173, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060681(n) is the largest difference between consecutive divisors of n. */

#include <stdint.h>
#include <stdlib.h>

/* helper: compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* helper: generate all divisors of a number given its prime factorization */
static void gen_divs_rec(const uint64_t *primes, const int *exps,
                         int k, int idx, uint64_t cur,
                         uint64_t *arr, size_t *cnt) {
    if (idx == k) {
        arr[(*cnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    int e = exps[idx];
    uint64_t val = 1;
    for (int i = 0; i <= e; ++i) {
        gen_divs_rec(primes, exps, k, idx + 1, cur * val, arr, cnt);
        val *= p;
    }
}

/* compute the largest difference between successive divisors of n */
static seq_t max_gap_of(uint64_t n) {
    if (n == 1) return 0;

    /* factor n */
    uint64_t primes[16];
    int exps[16];
    int k = 0;
    uint64_t tmp = n;
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        primes[k] = 2;
        exps[k]   = e;
        ++k;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[k] = p;
            exps[k]   = e;
            ++k;
        }
    }
    if (tmp > 1) {
        primes[k] = tmp;
        exps[k]   = 1;
        ++k;
    }

    /* generate all divisors */
    uint64_t divs[256];
    size_t div_cnt = 0;
    gen_divs_rec(primes, exps, k, 0, 1, divs, &div_cnt);

    /* sort divisors */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* maximum gap */
    uint64_t maxdiff = 0;
    for (size_t i = 1; i < div_cnt; ++i) {
        uint64_t diff = divs[i] - divs[i - 1];
        if (diff > maxdiff) maxdiff = diff;
    }
    return (seq_t)maxdiff;
}

/* compute Möbius function μ(n) for unsigned 64‑bit integer */
static int mobius_of(uint64_t n) {
    if (n == 1) return 1;
    int cnt = 0;
    /* factor n */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            if (e > 1) return 0;       /* square factor */
            ++cnt;
        }
    }
    if (n > 1) ++cnt;                     /* remaining prime factor */
    return (cnt % 2 == 0) ? 1 : -1;
}

/* main sequence function requested by the build system */
seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    seq_t g_n = max_gap_of(nn);

    /* factor nn to generate its divisors */
    uint64_t primes[16];
    int exps[16];
    int k = 0;
    uint64_t tmp = nn;
    if (tmp % 2 == 0) {
        int e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        primes[k] = 2;
        exps[k]   = e;
        ++k;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[k] = p;
            exps[k]   = e;
            ++k;
        }
    }
    if (tmp > 1) {
        primes[k] = tmp;
        exps[k]   = 1;
        ++k;
    }

    uint64_t divs[256];
    size_t div_cnt = 0;
    gen_divs_rec(primes, exps, k, 0, 1, divs, &div_cnt);

    /* sum over divisors: Σ μ(d)·g(n/d) */
    long long sum = 0;      /* signed to allow negative terms */
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        int mu = mobius_of(d);
        uint64_t m = nn / d;
        seq_t gm = max_gap_of(m);
        sum += (long long)mu * (long long)gm;
    }

    long long result = (long long)g_n - sum;
    return (seq_t)result;  /* final value assumed non‑negative */
}
