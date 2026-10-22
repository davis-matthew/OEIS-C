/*
 * A344339  a(n) is the minimal number of terms of A332520 that need to be
 *          combined with the bitwise OR operator in order to give n.
 * https://oeis.org/A344339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the minimal number of A332520 terms whose bitwise OR
    equals n.  The computation is performed once for all n
    that can be represented in at most 16 bits (i.e. n < 2^16).
    It pre‑computes the minimal DNF size for all truth tables of
    4 variables (16 minterms) which suffices for all n up to 10000.
    The set of implicants for 4 variables is 3^4 = 81, so the
    DP works in about 5 000 000 simple operations. */
#include <stdint.h>

#define MAX_K     4                      /* number of variables */
#define N_MASKS  (1U << (1U << MAX_K))  /* 2^(2^4) = 65536 */

/* Precomputed minimal number of terms for each 16–bit mask. */
static int dp[N_MASKS];

/* One-time initialization flag */
static int initialized = 0;

static void precompute(void)
{
    /* Generate all 3^4 implicant masks. */
    uint16_t implicants[81];
    int icnt = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d) {
                    uint8_t pat[4] = {(uint8_t)a, (uint8_t)b,
                                      (uint8_t)c, (uint8_t)d};
                    uint16_t m = 0;
                    for (uint16_t idx = 0; idx < (1U << MAX_K); ++idx) {
                        int ok = 1;
                        for (int p = 0; p < MAX_K; ++p) {
                            if (pat[p] == 2) continue;          /* don't care */
                            if (((idx >> p) & 1U) != pat[p]) {
                                ok = 0;
                                break;
                            }
                        }
                        if (ok) m |= (1U << idx);
                    }
                    implicants[icnt++] = m;                     /* 81 indices */
                }

    /* DP: minimal number of terms to cover each mask. */
    dp[0] = 0;
    for (uint32_t mask = 1; mask < N_MASKS; ++mask) {
        int best = 255;    /* large enough as INF */
        for (int i = 0; i < icnt; ++i) {
            uint16_t im = implicants[i];
            if ((im & mask) == im) {
                uint32_t rem = mask ^ im;   /* subset removal */
                int val = dp[rem] + 1;
                if (val < best) best = val;
            }
        }
        dp[mask] = best;
    }
    initialized = 1;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (!initialized)
        precompute();

    /* n is assumed to fit in 16 bits for the precomputed table. */
    return (seq_t)dp[(uint16_t)n];
}
