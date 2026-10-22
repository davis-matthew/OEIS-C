/*
 * A366719  Smallest prime dividing 12^n + 1.
 * https://oeis.org/A366719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime dividing 12^n + 1
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 511, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   build_primes_upto, isPrimeTrial, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* Cache of primes generated once */
    static bool primes_ready = false;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt    = 0;

    if (!primes_ready) {
        /* 2 000 000 gives primes up to ~2·10⁶ (~148 933 primes) */
        build_primes_upto((idx_t)2000000, &prime_list, &prime_cnt);
        primes_ready = true;
    }

    if (n == 0) return (seq_t)2;           /* 12^0 + 1 = 2 */

    /* Search among cached primes (skip 2) */
    for (size_t i = 1; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        /* Ensure p fits in seq_t; otherwise skip */
        if ((seq_t)p != p) continue;

        if (p == 2) continue;               /* 12^n is even for n>0 */

        seq_t mod_val = mod_pow((seq_t)12, n, (seq_t)p);
        if (mod_val == (seq_t)p - (seq_t)1) return (seq_t)p;
    }

    /* If not found, continue searching with larger odd primes */
    uint64_t cand = prime_list[prime_cnt - 1];
    if (cand % 2 == 0) cand++;              /* start with an odd candidate */

    for (;;) {
        if (isPrimeTrial((seq_t)cand)) {
            /* Only use candidates that fit into seq_t */
            if ((seq_t)cand == cand) {
                seq_t mod_val = mod_pow((seq_t)12, n, (seq_t)cand);
                if (mod_val == (seq_t)cand - (seq_t)1) return (seq_t)cand;
            }
        }
        cand += 2;                          /* next odd candidate */
    }
}
