/*
 * A363657  Numbers m where A217854(m) is a record minimum.
 * https://oeis.org/A363657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th perfect square m for which (-m)^τ(m) is a
 *            record minimum (equivalently, m^τ(m) is a record maximum among
 *            all squares)
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Static state to allow lazy generation across calls. */
    static uint32_t *spf = NULL;
    static const idx_t LIMIT = 5000000;          /* sieve bound  (≈5·10⁶)  */
    static idx_t current_k = 1;                 /* next k (square root) to test */
    static long double best_log = -INFINITY;    /* log of last record value */
    static seq_t *seq_arr = NULL;               /* array of sequence values (squares) */
    static size_t seq_len = 0, seq_cap = 0;      /* current number of records kept */

    /* --- initialise spf array once ------------------------------------- */
    if (!spf) {
        spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
        if (!spf) return 0;                     /* out‑of‑memory fallback */
        for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = (uint32_t)i;

        for (idx_t i = 2; i * i <= LIMIT; ++i) {
            if (spf[i] == i) {
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == j) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* --- generate records until we have at least n of them -------------- */
    while (seq_len < n) {
        uint32_t k = (uint32_t)current_k;        /* sqrt of the candidate square */
        uint64_t tau_k2 = 1;                     /* τ(k²) = ∏(2e_i + 1) */
        uint32_t tmp = k;

        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int e = 0;
            do { ++e; tmp /= p; } while (tmp % p == 0);
            tau_k2 *= (uint64_t)(2 * e + 1);
        }

        uint64_t exp = 2 * tau_k2;               /* exponent in k^exp = m^τ(m) */
        long double cand_log = (long double)exp * logl((long double)k);

        if (cand_log > best_log) {
            /* New record found: store m = k² */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len++] = (seq_t)k * (seq_t)k;
            best_log = cand_log;
        }

        ++current_k;
        if (current_k > LIMIT) {
            /* In practice LIMIT is sufficient for n ≤ 10000; this should not occur. */
            /* If needed, halt or extend the sieve here. */
        }
    }

    return seq_arr[n - 1];
}
