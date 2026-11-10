/*
 * A286533  Restricted growth sequence of A278533 (prime-signature of
 *          A253563).
 * https://oeis.org/A286533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence of the prime signature (sorted
 *            exponent multiset) of A253563(n); computed via exponent vectors,
 *            no values needed (A253563(2n): last prime exponent +1;
 *            A253563(2n+1): last prime exponent -1 and next prime added).
 *            a(0)=1
 * Range:     u8 n <= 32899, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 120 terms through n = 9487 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SIGLEN 70

static unsigned char *g_sigs = NULL;   /* distinct signatures, SIGLEN bytes each */
static size_t g_nsig = 0, g_capsig = 0;
static unsigned int *g_rg = NULL;      /* rg value per index */
static size_t g_done = 0, g_caprg = 0;

static unsigned int A286533_id(const unsigned char *sig) {
    size_t i;
    for (i = 0; i < g_nsig; i++)
        if (memcmp(g_sigs + i * SIGLEN, sig, SIGLEN) == 0) return (unsigned int)(i + 1);
    if (g_nsig == g_capsig) {
        g_capsig = g_capsig ? g_capsig * 2 : 64;
        g_sigs = (unsigned char *)realloc(g_sigs, g_capsig * SIGLEN);
    }
    memcpy(g_sigs + g_nsig * SIGLEN, sig, SIGLEN);
    g_nsig++;
    return (unsigned int)g_nsig;
}

static unsigned int A286533_term(idx_t m) {
    unsigned char e[SIGLEN + 2];
    unsigned char sig[SIGLEN];
    int k = 0, i, j, top;
    memset(sig, 0, sizeof(sig));
    if (m >= 1) {
        top = 63;
        while (!((m >> top) & 1)) top--;
        e[0] = 1; k = 1;
        for (i = top - 1; i >= 0; i--) {
            if ((m >> i) & 1) {
                e[k - 1]--;
                e[k] = 1;
                k++;
            } else {
                e[k - 1]++;
            }
        }
        int c = 0;
        for (i = 0; i < k; i++) {
            if (e[i] == 0) continue;
            unsigned char v = e[i];
            j = c++;
            while (j > 0 && sig[j - 1] < v) { sig[j] = sig[j - 1]; j--; }
            sig[j] = v;
        }
    }
    return A286533_id(sig);
}

seq_t a(idx_t n) {
    if (n >= g_done) {
        if ((size_t)n + 1 > g_caprg) {
            g_caprg = (size_t)n + 1 + g_caprg;
            g_rg = (unsigned int *)realloc(g_rg, g_caprg * sizeof(unsigned int));
        }
        while (g_done <= (size_t)n) {
            g_rg[g_done] = A286533_term((idx_t)g_done);
            g_done++;
        }
    }
    return (seq_t)g_rg[n];
}
