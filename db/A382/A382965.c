/*
 * A382965  The number of non-unitary prime divisors of the n-th cubefree
 *          number that is not squarefree.
 * https://oeis.org/A382965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non-unitary prime divisors of the n-th cubefree
 *            number that is not squarefree. A(n) equals the count of prime
 *            factors of the required number whose exponent equals 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static caching state */
    static idx_t seq_len = 0;          /* number of terms computed so far */
    static size_t seq_cap = 0;         /* capacity of seq array */
    static seq_t *seq = NULL;          /* sequence values array */

    static uint64_t next_candidate = 2;/* next integer to test as a candidate */

    /* Smallest prime factor sieve (once) */
    static uint32_t *spf = NULL;
    static const size_t SPF_LIMIT = 10000000; /* 10 million; far enough for n ~ 10^4 */

    /* Build spf table on the first call */
    if (!spf) {
        spf = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
        if (!spf) return 0;                 /* out of memory – return 0 as safe default */
        for (size_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = 0;
        spf[0] = spf[1] = 1;
        for (size_t i = 2; i <= SPF_LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if (i * i <= SPF_LIMIT) {
                    for (uint64_t j = (uint64_t)i * i; j <= SPF_LIMIT; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Ensure we have enough room for the result array */
    if (seq_len < n) {
        size_t needed = (size_t)n;
        if (seq_cap < needed) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!seq) return 0;              /* out of memory – return 0 as safe default */
            seq_cap = new_cap;
        }

        /* Generate terms until we have 'n' terms. */
        while (seq_len < n) {
            uint64_t m = next_candidate++;
            int count = 0;          /* count of prime factors with exponent 2 */
            uint64_t t = m;
            bool not_cube_free = false;

            /* Factor m using the spf table.  Since spf only goes up to SPF_LIMIT,
               we assume m never exceeds this limit (true for n up to ~10^4). */
            while (t > 1) {
                uint32_t p = spf[t];
                /* Fallback: if t is prime > SPF_LIMIT, treat p = t (rare here). */
                if (p == 0) p = (uint32_t)t;

                int exp = 0;
                while (t % p == 0) {
                    t /= p;
                    ++exp;
                    if (exp > 2) {          /* exponent >= 3 => not cubefree */
                        not_cube_free = true;
                        break;
                    }
                }
                if (not_cube_free) break;
                if (exp == 2) ++count;
            }

            if (not_cube_free) continue; /* skip non cubefree numbers */
            if (count == 0) continue;    /* skip squarefree numbers */

            seq[seq_len++] = (seq_t)count;
        }
    }

    return seq[(size_t)(n - 1)];
}
