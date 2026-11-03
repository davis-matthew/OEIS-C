/*
 * A316557  Number of distinct integer averages of subsets of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A316557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct integer averages of all non‑empty
 *            subsets of the multiset of indices whose corresponding primes
 *            multiply to n. For each prime factor p^e of n the index i with
 *            prime(i)=p occurs e times in the multiset. All integer averages
 *            of subsets of this multiset are collected,...
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9594 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The parts of the partition with Heinz number n are the indices pi(p) of
   its prime factors p, with multiplicity. A sub-multiset is a choice of
   exponents 0..e_i for each distinct prime (a divisor of n); its average is
   (sum of chosen indices) / (number chosen). The distinct integer averages of
   the nonempty choices are counted. pi comes from a sieve that grows with n
   (the original had fixed tables for n <= 10000). */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *pi_tab = NULL;           /* pi_tab[x] = pi(x), x <= pi_lim */
static uint64_t pi_lim = 0;

static void ensure_pi(uint64_t n)
{
    if (n <= pi_lim) return;
    uint64_t lim = pi_lim ? 2 * pi_lim : 1024;
    while (lim < n) lim *= 2;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    uint32_t *t = (uint32_t *)malloc((lim + 1) * sizeof *t);
    if (!comp || !t) abort();
    for (uint64_t i = 2; i * i <= lim; ++i)
        if (!comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i <= lim; ++i) {
        if (i >= 2 && !comp[i]) ++c;
        t[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = t;
    pi_lim = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;              /* no parts, no averages */
    ensure_pi((uint64_t)n);

    uint64_t idx[64], m = (uint64_t)n;        /* distinct parts and their multiplicities */
    int e[64], k = 0;
    for (uint64_t p = 2; p * p <= m; ++p)
        if (m % p == 0) {
            idx[k] = pi_tab[p];
            e[k] = 0;
            while (m % p == 0) { m /= p; ++e[k]; }
            ++k;
        }
    if (m > 1) { idx[k] = pi_tab[m]; e[k] = 1; ++k; }

    /* walk all exponent vectors c (0 <= c_i <= e_i) like an odometer */
    int c[64] = {0};
    uint64_t *seen = NULL;
    size_t nseen = 0, cap = 0;
    for (;;) {
        int i = 0;
        while (i < k && c[i] == e[i]) c[i++] = 0;
        if (i == k) break;
        ++c[i];
        uint64_t sum = 0, cnt = 0;
        for (int j = 0; j < k; ++j) { sum += (uint64_t)c[j] * idx[j]; cnt += (uint64_t)c[j]; }
        if (sum % cnt) continue;
        uint64_t avg = sum / cnt;
        size_t s = 0;
        while (s < nseen && seen[s] != avg) ++s;
        if (s < nseen) continue;
        if (nseen == cap) {
            cap = cap ? 2 * cap : 16;
            uint64_t *t = (uint64_t *)realloc(seen, cap * sizeof *t);
            if (!t) abort();
            seen = t;
        }
        seen[nseen++] = avg;
    }
    free(seen);
    return (seq_t)nseen;
}
