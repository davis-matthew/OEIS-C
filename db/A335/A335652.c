/*
 * A335652  Numbers k such that Omega(k+1) = Omega(k) + 2, where Omega(k) =
 *          A001222(k) is the number of prime factors of k with multiplicity.
 * https://oeis.org/A335652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that Omega(k+1)=Omega(k)+2 by brute‑force search */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;       /* smallest prime factor table   */
static idx_t spf_limit = 0;        /* current size of spf table      */

/* build a fresh SPF table up to 'limit' (inclusive) */
static void build_spf(idx_t limit)
{
    size_t lim = (size_t)limit;
    uint32_t *ptr = (uint32_t *)realloc(spf, (lim + 1) * sizeof(uint32_t));
    if (!ptr) return;                /* out of memory – give up */
    spf = ptr;
    for (size_t i = 0; i <= lim; ++i)
        spf[i] = 0;
    if (lim >= 1) spf[1] = 1;
    for (size_t i = 2; i <= lim; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= lim) {
                for (size_t j = i * i; j <= lim; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* make sure the SPF table covers at least 'need' */
static void ensure_spf(idx_t need)
{
    if (spf_limit >= need) return;
    idx_t new_limit = spf_limit ? spf_limit * 2 : need;
    if (new_limit < need) new_limit = need;
    build_spf(new_limit);
}

/* count Ω(n) with the current SPF table */
static inline int omega_uint64(uint64_t n)
{
    int cnt = 0;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        while (n % p == 0) {
            n /= p;
            ++cnt;
        }
    }
    return cnt;
}

/* sequence entry */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, safe fallback */

    idx_t k = 1;
    idx_t found = 0;

    while (found < n) {
        ensure_spf(k + 1);
        if (omega_uint64((uint64_t)(k + 1)) == omega_uint64((uint64_t)k) + 2) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
    return (seq_t)0;                   /* unreachable */
}
