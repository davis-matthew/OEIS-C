/*
 * A282434  Positions of records in A282442.
 * https://oeis.org/A282434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th position m at which A282443(m) (equivalently
 *            A282442(m) = A282443(m) + 1) sets a new record. The code finds
 *            these by searching m = 1, 2, 3, ... and simulating the staircase
 *            walk for each m. The walk is simulated in jumps: when a pair of
 *            moves (up by step s, down by step s+1) uses...
 * Range:     u8 n <= 48, u16 n <= 931, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 980 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned long long *A282434_rec = 0;
static size_t A282434_cnt = 0, A282434_cap = 0;
static unsigned long long A282434_m = 0;      /* last m examined */
static unsigned long long A282434_best = 0;   /* current record value */

/* Largest step size taken on a staircase of m steps (A282443). */
static unsigned long long A282434_maxstep(unsigned long long m) {
    unsigned long long p = 0;     /* position after the last down phase */
    unsigned long long s = 1;     /* next (odd) up step size */
    unsigned long long last = 0;  /* largest step size taken so far */
    for (;;) {
        unsigned long long u = (m - p) / s;
        if (u == 0) break;                       /* up step fails: done */
        if (p >= u) {
            /* Pairs j = 0..J keep the same u, each makes p -= u, s += 2 */
            unsigned long long A = (m - p - u * s) / u;
            unsigned long long B = (p - u) / u;
            unsigned long long J = A < B ? A : B;
            p -= (J + 1) * u;
            s += 2 * (J + 1);
            last = s - 1;                        /* last down step taken */
        } else {
            unsigned long long q = p + u * s;
            unsigned long long d;
            last = s;
            d = q / (s + 1);
            if (d == 0) break;                   /* down step fails: done */
            p = q % (s + 1);
            last = s + 1;
            s += 2;
        }
    }
    return last;
}

seq_t a(idx_t n) {
    while (A282434_cnt < (size_t)n) {
        unsigned long long m = ++A282434_m;
        unsigned long long v = A282434_maxstep(m);
        if (v > A282434_best) {
            A282434_best = v;
            if (A282434_cnt == A282434_cap) {
                size_t nc = A282434_cap ? A282434_cap * 2 : 64;
                unsigned long long *np = (unsigned long long *)realloc(
                    A282434_rec, nc * sizeof(unsigned long long));
                if (!np) return (seq_t)0;
                A282434_rec = np;
                A282434_cap = nc;
            }
            A282434_rec[A282434_cnt++] = m;
        }
    }
    return (seq_t)A282434_rec[n - 1];
}
