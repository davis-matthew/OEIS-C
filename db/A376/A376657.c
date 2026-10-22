/*
 * A376657  Number of integer factorizations of n into nonsquarefree factors >
 *          1.
 * https://oeis.org/A376657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1,
 *            each factor being non‑squarefree
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *spf       = NULL;
static unsigned char *nonsq = NULL;
static uint64_t max_pre    = 0;

/* Build smallest‑prime‑factor sieve and flag non‑squarefree numbers up to N */
static void ensure_sieve(uint64_t N)
{
    if (N <= max_pre) return;
    uint64_t mx = N;
    uint32_t *new_spf = (uint32_t *)malloc((mx + 1) * sizeof(uint32_t));
    unsigned char *new_nonsq = (unsigned char *)malloc((mx + 1) * sizeof(unsigned char));
    if (!new_spf || !new_nonsq) return;   /* malloc failure – silently ignore */

    for (uint64_t i = 0; i <= mx; ++i) {
        new_spf[i] = 0;
        new_nonsq[i] = 0;
    }
    new_spf[0] = 0; new_spf[1] = 1;

    /* smallest prime factor sieve */
    for (uint64_t i = 2; i <= mx; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i <= mx) {
                for (uint64_t j = i * i; j <= mx; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* flag non‑squarefree numbers */
    for (uint64_t i = 2; i <= mx; ++i) {
        uint64_t x = i;
        bool ns = false;
        while (x > 1) {
            uint32_t p = new_spf[x];
            uint64_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                if (cnt >= 2) { ns = true; break; }
            }
            if (ns) break;
        }
        new_nonsq[i] = ns ? 1 : 0;
    }

    if (spf)  free(spf);
    if (nonsq) free(nonsq);
    spf = new_spf;
    nonsq = new_nonsq;
    max_pre = mx;
}

/* Recursive counting of factorizations with factors >= minF */
static seq_t rec(uint64_t x, uint64_t minF)
{
    if (x == 1) return (seq_t)1;
    seq_t res = 0;
    for (uint64_t d = minF; d <= x / d; ++d) {
        if (x % d == 0) {
            uint64_t f1 = d;
            uint64_t f2 = x / d;
            if (nonsq[f1]) res += rec(x / f1, f1);
            if (f2 != f1 && f2 >= minF && nonsq[f2]) res += rec(x / f2, f2);
        }
    }
    if (nonsq[x] && x >= minF) res += (seq_t)1;
    return res;
}

/* Public function required by the judge */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_sieve((uint64_t)n);
    return rec((uint64_t)n, (uint64_t)2);
}
