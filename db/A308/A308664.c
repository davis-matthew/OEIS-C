/*
 * A308664  Numbers k such that tau(k) and phi(k) are the legs of a
 *          Pythagorean triple.
 * https://oeis.org/A308664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308664: Numbers k such that tau(k) and phi(k) are the legs of a Pythagorean triple */
/* The function builds the whole finite sequence once, then returns the n‑th term. */
/* (The maximum possible k is below 5000, because phi(k) ≤ 2304 and phi(k) ≥ k/2 for powers of 2.) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- integer square root for 64‑bit values ----- */
static uint64_t isqrt_uint64(uint64_t x) {
    uint64_t lo = 0, hi = 65536;          /* sqrt(2^32‑1) < 65536  */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ----- build the sequence only once ----- */
static void build_sequence(seq_t **out, size_t *len) {
    const size_t NMAX = 5000;             /* sufficient upper bound for k */

    /* auxiliary arrays for prime factorisation */
    uint32_t *spf   = (uint32_t*)malloc((NMAX + 1) * sizeof(uint32_t));
    uint64_t *phi   = (uint64_t*)malloc((NMAX + 1) * sizeof(uint64_t));
    uint32_t *tau   = (uint32_t*)malloc((NMAX + 1) * sizeof(uint32_t));

    /* smallest prime factor sieve */
    for (size_t i = 0; i <= NMAX; ++i) spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i <= NMAX; ++i)
        if (spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= NMAX; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;

    /* initialise φ and τ */
    phi[1] = 1;
    tau[1] = 1;

    for (size_t n = 2; n <= NMAX; ++n) {
        uint32_t m = n;
        uint64_t phi_n = n;
        uint32_t tau_n = 1;
        while (m > 1) {
            uint32_t p = spf[m];
            uint32_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            phi_n -= phi_n / p;
            tau_n *= (exp + 1);
        }
        phi[n] = phi_n;
        tau[n] = tau_n;
    }

    /* collect the valid k values */
    size_t cap = 16;
    seq_t *list = (seq_t*)malloc(cap * sizeof(seq_t));
    size_t count = 0;

    for (size_t n = 1; n <= NMAX; ++n) {
        uint64_t a = (uint64_t)tau[n];
        uint64_t b = (uint64_t)phi[n];
        uint64_t s = a * a + b * b;
        uint64_t w = isqrt_uint64(s);
        if (w * w == s) {                     /* perfect square, triple exists */
            if (count == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t*)realloc(list, cap * sizeof(seq_t));
                list = tmp;
            }
            list[count++] = (seq_t)n;
        }
    }

    free(spf);
    free(phi);
    free(tau);

    *out = list;
    *len = count;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* lazy static cache */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!seq) {
        build_sequence(&seq, &seq_len);
    }

    if (n == 0 || (idx_t)seq_len < n)
        return (seq_t)0; /* undefined for out‐of‐range indices */

    return seq[(size_t)n - 1];
}
