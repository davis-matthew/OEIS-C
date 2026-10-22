/*
 * A322075  Number of factorizations of n into nonprime squarefree numbers >
 *          1.
 * https://oeis.org/A322075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements counting factorizations of n
 * into nonprime squarefree numbers > 1.
 * The function counts unordered factorizations,
 * treating the empty factorization as one for n = 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 10000               /* maximum n that will be queried */

/* smallest prime factor for each number up to LIMIT */
static uint64_t spf[LIMIT + 1];
/* list of composite square‑free numbers up to LIMIT */
static int nonprime_sqf[LIMIT];
static int nonprime_cnt = 0;
/* one‑time initialisation flag */
static bool init_done = false;

/* initialise spf and the list of nonprime squarefree numbers */
static void init(void) {
    if (init_done) return;

    /* initialise spf array: spf[n] = n initially */
    for (uint64_t i = 0; i <= LIMIT; ++i) spf[i] = i;

    /* sieve for spf */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* collect all composite square‑free numbers */
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        uint64_t v = i;
        int prev = 0;
        int distinct = 0;
        bool squarefree = true;
        while (v > 1) {
            int p = (int)spf[v];
            if (p == prev) {          /* repeated prime factor -> not square‑free */
                squarefree = false;
                break;
            }
            prev = p;
            ++distinct;
            v /= p;
        }
        if (squarefree && distinct >= 2)            /* composite and square‑free */
            nonprime_sqf[nonprime_cnt++] = (int)i;
    }

    init_done = true;
}

/* recursive enumeration of unordered factorizations
 * cur   : current part of the integer to factor
 * minf  : smallest allowed factor for the current position
 */
static seq_t rec(idx_t cur, idx_t minf) {
    if (cur == (idx_t)1) return (seq_t)1;          /* one way to finish the factorisation */
    seq_t res = (seq_t)0;
    for (int i = 0; i < nonprime_cnt; ++i) {
        idx_t f = (idx_t)nonprime_sqf[i];
        if (f < minf) continue;                    /* enforce nondecreasing order */
        if (f > cur) break;                        /* no larger factors will fit */
        if (cur % f == 0) {
            res += rec(cur / f, f);
        }
    }
    return res;
}

/* public function required by the build */
seq_t a(idx_t n) {
    init();
    /* n is guaranteed >= 1 in the OEIS definition */
    return rec(n, (idx_t)6);   /* smallest nonprime square‑free number is 6 */
}
