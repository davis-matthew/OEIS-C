/*
 * A343415  Primes in A343414.
 * https://oeis.org/A343415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime among A343414(m), m >= 0, where
 *            A343414(m) = Sum_{i=1..m+1} prime(i)*Sum_{j=0..i-1}
 *            binomial(m,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 21 terms through n = 21 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *A343415_pr = NULL;
static size_t A343415_psz = 0, A343415_pcap = 0;

static uint32_t A343415_prime(size_t k)
{
    if (A343415_pcap == 0) {
        A343415_pcap = 64;
        A343415_pr = (uint32_t *)malloc(A343415_pcap * sizeof(uint32_t));
        if (!A343415_pr) abort();
    }
    while (A343415_psz < k) {
        uint32_t cand = A343415_psz ? A343415_pr[A343415_psz - 1] + 1 : 2;
        for (;; ++cand) {
            bool ok = true;
            for (size_t i = 0; i < A343415_psz; ++i) {
                uint64_t p = A343415_pr[i];
                if (p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (A343415_psz == A343415_pcap) {
            A343415_pcap <<= 1;
            A343415_pr = (uint32_t *)realloc(A343415_pr, A343415_pcap * sizeof(uint32_t));
            if (!A343415_pr) abort();
        }
        A343415_pr[A343415_psz++] = cand;
    }
    return A343415_pr[k - 1];
}

static seq_t A343414_a(idx_t n)
{
    seq_t total = 0, c = 1, prefix = 0;
    for (idx_t k = 0; k <= n; ++k) {
        prefix += c;
        total += (seq_t)A343415_prime((size_t)k + 1) * prefix;
        if (k < n) {
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return total;
}

/* 1 if seq_t is arbitrary precision, 0 if native fixed width */
static int A343415_bigmode = -1;
static bool A343415_is_big(void)
{
    if (A343415_bigmode < 0) {
        seq_t t = 1;
        for (int i = 0; i < 140; ++i) t *= 2;
        A343415_bigmode = (t != 0) ? 1 : 0;
    }
    return A343415_bigmode == 1;
}

static seq_t A343415_addm(seq_t x, seq_t y, seq_t m)
{
    seq_t t = m - y;
    if (x >= t) return x - t;
    return x + y;
}

static seq_t A343415_mulm(seq_t x, seq_t y, seq_t m)
{
    if (A343415_bigmode == 1) return x * y % m;
    if (x == 0 || y == 0) return 0;
    seq_t p = x * y;
    if (p / y == x) return p % m;
    seq_t r = 0;
    while (y != 0) {
        if ((y & 1) != 0) r = A343415_addm(r, x, m);
        x = A343415_addm(x, x, m);
        y >>= 1;
    }
    return r;
}

static seq_t A343415_powm(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b %= m;
    while (e != 0) {
        if ((e & 1) != 0) r = A343415_mulm(r, b, m);
        e >>= 1;
        if (e != 0) b = A343415_mulm(b, b, m);
    }
    return r;
}

static bool A343415_isprime(seq_t n)
{
    A343415_is_big();
    if (n < 2) return false;
    A343415_prime(1200);
    /* trial division by primes < 1000 */
    size_t i = 0;
    for (; A343415_pr[i] < 1000; ++i) {
        if (n == (seq_t)A343415_pr[i]) return true;
        if (n % (seq_t)A343415_pr[i] == 0) return false;
    }
    if (n < 1000000) return true;
    if (A343415_bigmode == 1) {
        /* grouped trial division by primes in [1000, ~9700] */
        while (i < 1200) {
            uint64_t P = 1;
            size_t j = i;
            while (j < 1200 && P < ((uint64_t)1 << 44)) P *= A343415_pr[j++];
            uint64_t r = (uint64_t)(long long)(n % (seq_t)P);
            for (size_t t = i; t < j; ++t)
                if (r % A343415_pr[t] == 0) return false;
            i = j;
        }
    }
    seq_t nm1 = n - 1;
    seq_t d = nm1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const unsigned bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t b = 0; b < sizeof(bases) / sizeof(bases[0]); ++b) {
        seq_t x = A343415_powm((seq_t)bases[b], d, n);
        if (x == 1 || x == nm1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; ++r) {
            x = A343415_mulm(x, x, n);
            if (x == nm1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static idx_t *A343415_found = NULL;
static size_t A343415_fsz = 0, A343415_fcap = 0;
static idx_t A343415_next = 0;

seq_t a(idx_t n)
{
    while (A343415_fsz < (size_t)n) {
        idx_t m = A343415_next++;
        seq_t v = A343414_a(m);
        if (A343415_isprime(v)) {
            if (A343415_fsz == A343415_fcap) {
                A343415_fcap = A343415_fcap ? A343415_fcap * 2 : 64;
                A343415_found = (idx_t *)realloc(A343415_found, A343415_fcap * sizeof(idx_t));
                if (!A343415_found) abort();
            }
            A343415_found[A343415_fsz++] = m;
        }
    }
    return A343414_a(A343415_found[(size_t)n - 1]);
}
