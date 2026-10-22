/*
 * A389479  a(n) is the Frobenius number for the set { binomial(m,k), k=1..m-1
 *          } where m = A024619(n).
 * https://oeis.org/A389479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = -m + Σ_{p^d|m} binomial(m,p^d)*(p-1), where m =
 *            A024619(n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Adapted for the OEIS build – uses the provided utilities. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, safe */

    /* Static caching of previously computed terms */
    static size_t cache_cap = 0;           /* Capacity of arrays */
    static size_t cache_size = 0;          /* Number of terms cached so far */

    static idx_t *m_cache = NULL;         /* Cached m-values (A024619) */
    static seq_t *a_cache = NULL;         /* Cached a-values */

    static idx_t next_val = 2;             /* Candidate integer for next m */

    /* Ensure enough space for n terms */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        m_cache = (idx_t *)realloc(m_cache, new_cap * sizeof(idx_t));
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms up to index n if required */
    while (cache_size < n) {
        /* Find next integer that is NOT a prime power (and not 1) */
        idx_t m = 0;
        while (true) {
            uint64_t primes[64], exps[64];
            size_t pc;
            factor_u64((uint64_t)next_val, primes, exps, &pc);
            if (pc > 1) {                 /* m has at least two distinct prime factors */
                m = next_val;
                break;
            }
            ++next_val;
        }

        /* Cache the discovered m */
        m_cache[cache_size] = m;

        /* Compute a(m) using the given formula */
        seq_t sum = 0;
        {
            uint64_t primes[64], exps[64];
            size_t pc;
            factor_u64((uint64_t)m, primes, exps, &pc);

            for (size_t i = 0; i < pc; ++i) {
                uint64_t p = primes[i];
                uint64_t e = exps[i];
                uint64_t pk = 1;
                for (uint64_t d = 1; d <= e; ++d) {
                    pk *= p;                                 /* pk = p^d */
                    seq_t bin = binomial((idx_t)m, (idx_t)pk);
                    sum += bin * ((seq_t)(p - 1));
                }
            }
        }
        seq_t a_val = sum - (seq_t)m;                     /* a(n) = sum - m */
        a_cache[cache_size] = a_val;

        ++cache_size;
        ++next_val;                                       /* advance to next candidate */
    }

    return a_cache[n - 1];
}
