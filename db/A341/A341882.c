/*
 * A341882  Number of ordered factorizations of n into 6 factors > 1.
 * https://oeis.org/A341882
 *
 * Offset:    64    (a(64) is the first term)
 * Keywords:  nonn
 * Formula:   counts the number of ordered factorizations of n into * 6
 *            factors > 1 using a DP over prime exponents
 * Range:     u8 n <= 575, u16 n <= 9479, u32 n <= 9479, u64 n <= 9479,
 *            u128 n <= 9479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Pre‑compute how many ways to distribute an exponent e
 * among 6 positions, marking which positions receive at least one
 * copy of the prime.  ways[e][mask] is the number of 6‑tuples
 * (a,b,c,d,e,f,g) with a+b+...+g = e and the given mask. */
#define MAX_EXP 20
static uint32_t ways[MAX_EXP + 1][64];
static bool ways_initialized = false;

static void init_ways(void) {
    if (ways_initialized) return;
    for (int e = 0; e <= MAX_EXP; ++e)
        for (int m = 0; m < 64; ++m)
            ways[e][m] = 0;

    /* e = 0: only the zero tuple */
    ways[0][0] = 1;

    for (int e = 1; e <= MAX_EXP; ++e) {
        for (int a = 0; a <= e; ++a) {
            int rem1 = e - a;
            for (int b = 0; b <= rem1; ++b) {
                int rem2 = rem1 - b;
                for (int c = 0; c <= rem2; ++c) {
                    int rem3 = rem2 - c;
                    for (int d = 0; d <= rem3; ++d) {
                        int rem4 = rem3 - d;
                        for (int f = 0; f <= rem4; ++f) {
                            int g = rem4 - f;   /* remaining exponent */
                            int mask = 0;
                            if (a) mask |= 1 << 0;
                            if (b) mask |= 1 << 1;
                            if (c) mask |= 1 << 2;
                            if (d) mask |= 1 << 3;
                            if (f) mask |= 1 << 4;
                            if (g) mask |= 1 << 5;
                            ways[e][mask] += 1;
                        }
                    }
                }
            }
        }
    }
    ways_initialized = true;
}

/* A(idx_t n) returns the number of ordered factorizations of n
 * into exactly six factors, each > 1.  The function works for any
 * n ≥ 64; for smaller n it returns 0 (the behaviour below the offset does
 * not matter).  It can be called in any order. */
seq_t a(idx_t n) {
    if (n < 64) return 0;          /* below offset – undefined for this sequence */

    init_ways();

    /* Factorise n by trial division (n ≤ 10 000, small enough).  */
    uint64_t tmp = (uint64_t)n;
    int exps[16];          /* maximum 16 distinct primes for n ≤ 10 000 */
    int excnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                cnt++;
            }
            exps[excnt++] = cnt;
        }
    }
    if (tmp > 1) {
        exps[excnt++] = 1;   /* remaining prime factor */
    }

    /* DP over 6‑factor masks.  dp[mask] = number of ways to reach this
     * mask after processing some primes.  The bit i is set iff
     * factor i has received at least one prime factor. */
    seq_t dp[64] = {0};
    dp[0] = 1;

    for (int i = 0; i < excnt; ++i) {
        int e = exps[i];
        seq_t ndp[64] = {0};
        for (int m1 = 0; m1 < 64; ++m1) {
            if (dp[m1] == 0) continue;
            for (int m2 = 0; m2 < 64; ++m2) {
                uint32_t w = ways[e][m2];
                if (w == 0) continue;
                int newmask = m1 | m2;
                ndp[newmask] += dp[m1] * (seq_t)w;
            }
        }
        for (int m = 0; m < 64; ++m) dp[m] = ndp[m];
    }

    return dp[63];   /* mask 0b111111: all six factors non‑trivial */
}
