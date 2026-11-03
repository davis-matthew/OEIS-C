/*
 * A325989  Number of perfect factorizations of n.
 * https://oeis.org/A325989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of perfect factorizations of n. The implementation
 *            precomputes the number of perfect factorizations for every prime
 *            power p^e (e up to 64). For a composite n, a(n) is the product
 *            of the values for its prime‑power factors. The computation of
 *            the prime‑power count enumerates all parti...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9666 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* test whether the multiset of parts produces a unique representation
   of each integer 0..e */
static int valid_parts(const int *parts, int depth, int e) {
    int mult[65] = {0};                 /* multiplicity of each part value */
    for (int i = 0; i < depth; ++i) {
        int v = parts[i];
        mult[v] += 1;
    }

    int dp[65]     = {0};
    int newdp[65]  = {0};
    dp[0] = 1;

    for (int val = 1; val <= e; ++val) {
        int c = mult[val];
        if (c == 0) continue;

        memset(newdp, 0, sizeof(newdp));
        for (int s = 0; s <= e; ++s) {
            if (!dp[s]) continue;
            for (int k = 0; k <= c; ++k) {
                int ns = s + k * val;
                if (ns > e) break;
                newdp[ns] += dp[s];
                if (newdp[ns] > 1) return 0;    /* duplicate sum found */
            }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* all sums 0..e must have exactly one representation */
    for (int s = 0; s <= e; ++s)
        if (dp[s] != 1) return 0;

    return 1;
}

/* enumerate all integer partitions of 'remaining', ensuring parts are
   nondecreasing.  When a partition is complete, test it with valid_parts. */
static void enum_parts(int remaining, int min_part,
                       int depth, int *parts,
                       seq_t *cnt, int e) {
    if (remaining == 0) {
        if (valid_parts(parts, depth, e))
            (*cnt) += (seq_t)1;
        return;
    }
    for (int part = min_part; part <= remaining; ++part) {
        parts[depth] = part;
        enum_parts(remaining - part, part, depth + 1,
                   parts, cnt, e);
    }
}

/* --- precomputation of f[e] : number of perfect factorizations of p^e --- */

static seq_t *f_table = NULL;          /* table f[e] */
static int   f_max_exp = 64;           /* maximum exponent covered */

/* function prototypes */
static void   prepare_f(void);
static int    valid_parts(const int *parts, int depth, int e);
static void   enum_parts(int remaining, int min_part,
                         int depth, int *parts,
                         seq_t *cnt, int e);

/* create f_table if not yet prepared */
static void prepare_f(void) {
    if (f_table != NULL) return;
    f_table = mem_new(seq_t, (f_max_exp + 1));
    mem_set(f_table, 0, (f_max_exp + 1) * sizeof(seq_t));
    f_table[0] = (seq_t)1;              /* not used, but harmless */

    int parts[64];
    for (int e = 1; e <= f_max_exp; ++e) {
        seq_t cnt = (seq_t)0;
        enum_parts(e, 1, 0, parts, &cnt, e);
        f_table[e] = cnt;
    }
}

/* --- public sequence function --- */

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;    /* a(1) = 1 */

    prepare_f();                            /* precompute f_table */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = (idx_t)2;
         p * p <= temp;  /* safe for n <= 10^9, and faster than sqrt() */
         ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result = result * f_table[exp];
        }
    }
    if (temp > (idx_t)1) {
        /* remaining prime factor with exponent 1 */
        result = result * f_table[1];
    }

    return result;
}
