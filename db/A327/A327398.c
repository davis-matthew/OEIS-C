/*
 * A327398  Maximum connected squarefree divisor of n.
 * https://oeis.org/A327398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   maximum connected squarefree divisor of n (by enumerating
 *            squarefree divisors)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/prime_pi.h"

/* Return the maximum connected squarefree divisor of n */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but return harmless value */
        return (seq_t)0;

    /* factor n, keeping only distinct primes */
    uint64_t primes[64];
    uint64_t exps[64];         /* exponent array needed by factor_u64 but unused here */
    size_t   pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* n = 1 -> only divisor is 1 */
    if (pcnt == 0)
        return (seq_t)1;

    seq_t best = (seq_t)1;     /* 1 is always a valid divisor */

    /* Enumerate all non‑empty subsets of distinct prime factors */
    uint64_t subset_cnt = ((uint64_t)1) << pcnt;
    for (uint64_t mask = 1; mask < subset_cnt; ++mask) {
        seq_t prod = (seq_t)1;
        uint32_t idxs[64];
        size_t sz = 0;

        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                prod = prod * (seq_t)primes[i];
                idxs[sz++] = (uint32_t)prime_pi(primes[i]);   /* vertex label = prime index */
            }
        }

        /* Connectivity test using DFS */
        bool connected = true;
        if (sz > 1) {
            bool visited[64] = { false };
            size_t stack[64];
            size_t sp = 0;

            visited[0] = true;
            stack[sp++] = 0;

            while (sp > 0) {
                size_t v = stack[--sp];
                for (size_t w = 0; w < sz; ++w) {
                    if (!visited[w] &&
                        gcd((seq_t)idxs[v], (seq_t)idxs[w]) > (seq_t)1) {
                        visited[w] = true;
                        stack[sp++] = w;
                    }
                }
            }

            for (size_t i = 0; i < sz; ++i) {
                if (!visited[i]) {
                    connected = false;
                    break;
                }
            }
        }

        if (connected && prod > best)
            best = prod;
    }

    return best;
}
