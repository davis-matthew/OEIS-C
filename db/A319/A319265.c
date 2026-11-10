/*
 * A319265  Numbers k such that A000607(k) is divisible by k.
 * https://oeis.org/A319265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search: a(n) is the n-th k >= 1 with A000607(k) == 0 (mod k).
 *            A000607 is computed exactly for all k <= T by the prime-parts
 *            partition DP dp[i] += dp[i-p] (p prime), using multi-limb
 *            integers with 62-bit limbs. T grows by a factor of 3/2 as needed
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A319265_MASK ((((uint64_t)1) << 62) - 1)

static uint64_t **A319265_limb = NULL;
static int A319265_nlimb = 0, A319265_climb = 0;
static unsigned char *A319265_len = NULL;
static idx_t A319265_T = 0;
static idx_t A319265_scanned = 0;
static idx_t *A319265_found = NULL;
static size_t A319265_nfound = 0, A319265_cap = 0;

static void A319265_free_all(void) {
    for (int j = 0; j < A319265_nlimb; j++) free(A319265_limb[j]);
    A319265_nlimb = 0;
    free(A319265_len);
    A319265_len = NULL;
}

static void A319265_add_limb(idx_t T) {
    if (A319265_nlimb == A319265_climb) {
        A319265_climb = A319265_climb ? A319265_climb * 2 : 8;
        A319265_limb = (uint64_t **)realloc(A319265_limb,
                                            (size_t)A319265_climb * sizeof(uint64_t *));
    }
    A319265_limb[A319265_nlimb++] = (uint64_t *)calloc((size_t)T + 1, sizeof(uint64_t));
}

static void A319265_build(idx_t T) {
    A319265_free_all();
    A319265_T = T;
    A319265_len = (unsigned char *)calloc((size_t)T + 1, 1);
    A319265_add_limb(T);
    A319265_limb[0][0] = 1;
    A319265_len[0] = 1;

    unsigned char *comp = (unsigned char *)calloc((size_t)T + 1, 1);
    for (idx_t i = 2; i * i <= T; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= T; j += i) comp[j] = 1;

    unsigned char *len = A319265_len;
    uint64_t **Lb = A319265_limb;
    for (idx_t p = 2; p <= T; p++) {
        if (comp[p]) continue;
        for (idx_t i = p; i <= T; i++) {
            unsigned lb = len[i - p];
            if (!lb) continue;
            unsigned la = len[i];
            unsigned nn = la > lb ? la : lb;
            uint64_t c = 0;
            for (unsigned j = 0; j < nn; j++) {
                uint64_t *L = Lb[j];
                uint64_t s = L[i] + L[i - p] + c;
                L[i] = s & A319265_MASK;
                c = s >> 62;
            }
            if (c) {
                if ((int)nn == A319265_nlimb) {
                    A319265_add_limb(T);
                    Lb = A319265_limb;
                }
                Lb[nn][i] = c;
                nn++;
            }
            len[i] = (unsigned char)nn;
        }
    }
    free(comp);
}

static bool A319265_divides(idx_t k) {
    int l = (int)A319265_len[k];
    uint64_t r = 0;
    for (int j = l - 1; j >= 0; j--) {
        uint64_t v = A319265_limb[j][k];
        r = ((r << 31) | (v >> 31)) % (uint64_t)k;
        r = ((r << 31) | (v & 0x7FFFFFFFULL)) % (uint64_t)k;
    }
    return r == 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A319265_nfound < (size_t)n) {
        if (A319265_scanned >= A319265_T)
            A319265_build(A319265_T ? A319265_T + A319265_T / 2 : 4096);
        for (idx_t k = A319265_scanned + 1; k <= A319265_T; k++) {
            if (A319265_divides(k)) {
                if (A319265_nfound == A319265_cap) {
                    A319265_cap = A319265_cap ? A319265_cap * 2 : 16;
                    A319265_found = (idx_t *)realloc(A319265_found,
                                                     A319265_cap * sizeof(idx_t));
                }
                A319265_found[A319265_nfound++] = k;
            }
            A319265_scanned = k;
        }
    }
    return (seq_t)A319265_found[n - 1];
}
