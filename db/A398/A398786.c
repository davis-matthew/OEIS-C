/*
 * A398786  a(n) is the number of times n appears in A091508.
 * https://oeis.org/A398786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   backward enumeration of the A091508 recursion. Start from b(n) =
 *            1 and walk back through all possible predecessors. The
 *            predecessor of b(s) is either b(s)-s, when gcd(s, b(s)) = 1, or
 *            b(s)*g, when g | s, g >= 2 and gcd(s/g, b(s)) = 1. No earlier
 *            term may be 1. For levels s <= 16, the remaining co...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define A398786_S 16

static uint32_t *A398786_tab[A398786_S + 1];
static uint64_t A398786_L[A398786_S + 1];
static int A398786_ready = 0;
static seq_t A398786_memo[65];
static unsigned char A398786_have[65];

static uint64_t A398786_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static int A398786_setup(void)
{
    unsigned s;
    A398786_L[1] = 1;
    A398786_tab[1] = (uint32_t *)malloc(sizeof(uint32_t));
    if (!A398786_tab[1]) return 0;
    A398786_tab[1][0] = 1;
    for (s = 2; s <= A398786_S; s++) {
        uint64_t Ls, Lp = A398786_L[s - 1], r;
        uint32_t *prev = A398786_tab[s - 1], *cur;
        unsigned divs[16], nd = 0, g;
        Ls = Lp / A398786_gcd(Lp, s) * s;
        A398786_L[s] = Ls;
        for (g = 2; g <= s; g++) if (s % g == 0) divs[nd++] = g;
        cur = (uint32_t *)malloc((size_t)Ls * sizeof(uint32_t));
        if (!cur) return 0;
        for (r = 0; r < Ls; r++) {
            uint64_t tot = 0, w = r % s;
            unsigned i;
            if (A398786_gcd(w, s) == 1)
                tot += prev[((r + Ls - s) % Ls) % Lp];
            for (i = 0; i < nd; i++) {
                unsigned q = s / divs[i];
                if (A398786_gcd(w % q, q) == 1)
                    tot += prev[(r * divs[i]) % Lp];
            }
            cur[r] = (uint32_t)tot;
        }
        A398786_tab[s] = cur;
    }
    A398786_ready = 1;
    return 1;
}

#define A398786_CAP ((uint64_t)1 << 62)

/* vs: exact value of b(s) unless big; vm: b(s) mod M */
static seq_t A398786_rec(unsigned s, uint64_t vs, int big, uint64_t vm, uint64_t M)
{
    seq_t total = 0;
    uint64_t w;
    unsigned g;
    if (s == 1) return (seq_t)1;
    if (s <= A398786_S &&
        (big || vs >= (uint64_t)s * (s + 1) / 2 + 1))
        return (seq_t)A398786_tab[s][vm % A398786_L[s]];
    w = vm % s;
    /* additive predecessor b(s-1) = b(s) - s, must be >= 2 */
    if (A398786_gcd(w, s) == 1 && (big || vs >= (uint64_t)s + 2))
        total += A398786_rec(s - 1, big ? vs : vs - s, big,
                             (vm + M - (uint64_t)s % M) % M, M);
    /* division predecessors b(s-1) = b(s) * g */
    for (g = 2; g <= s; g++) {
        unsigned q;
        uint64_t nvs = vs;
        int nbig = big;
        if (s % g) continue;
        q = s / g;
        if (A398786_gcd(w % q, q) != 1) continue;
        if (!nbig) {
            if (vs > A398786_CAP / g) nbig = 1;
            else nvs = vs * g;
        }
        total += A398786_rec(s - 1, nvs, nbig, (vm * g) % M, M);
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t r;
    uint64_t M = 1;
    unsigned k, top;
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n <= 64 && A398786_have[n]) return A398786_memo[n];
    if (!A398786_ready && !A398786_setup()) return (seq_t)0;
    top = (n > A398786_S) ? (unsigned)n : A398786_S;
    for (k = 2; k <= top; k++) M = M / A398786_gcd(M, k) * k;
    r = A398786_rec((unsigned)n, 1, 0, 1 % M, M);
    if (n <= 64) { A398786_memo[n] = r; A398786_have[n] = 1; }
    return r;
}
