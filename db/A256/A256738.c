/*
 * A256738  Largest number that is not the sum of distinct primes of the form
 *          12*k + 2*n + 1, or 0 if none exists.
 * https://oeis.org/A256738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the largest integer that cannot be expressed as a sum of
 *            distinct primes congruent to (2·n + 1) mod 12; a(n) = 0 if every
 *            positive integer is representable. The function computes this
 *            value by a bounded subset–sum dp for each residue class that
 *            actually contains infinitely many primes (r...
 * Range:     u8 none, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

#define LIMIT 20000

seq_t a(idx_t n)
{
    /* Compute the residue class r = (2*n + 1) mod 12. */
    idx_t r = (2 * (n % 12) + 1) % 12U;

    /* Primes >3 are only in residues 1,5,7,11.  Other residues yield 0. */
    if (r != 1U && r != 5U && r != 7U && r != 11U)
        return 0;

    /* Static cache for the four residue classes. */
    static seq_t cache[12] = {0};
    static bool ready[12] = {false};

    if (ready[r])
        return cache[r];

    /* ---------- Build primes up to LIMIT ---------- */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(LIMIT, &primes, &prime_cnt);

    /* ---------- Subset–sum DP for primes with prime % 12 == r ---------- */
    static bool reachable[LIMIT + 1];
    memset(reachable, 0, sizeof(reachable));
    reachable[0] = true;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p % 12 != r)
            continue;          /* only keep primes of the correct residue */

        /* p ≤ LIMIT by construction */
        int ip = (int)p;
        for (int s = LIMIT; s >= ip; --s) {
            if (reachable[s - ip])
                reachable[s] = true;
        }
    }

    /* ---------- Find largest non‑representable integer ---------- */
    seq_t res = 0;
    for (int s = LIMIT; s >= 1; --s) {
        if (!reachable[s]) {
            res = (seq_t)s;
            break;
        }
    }

    /* ---------- Clean up and cache result ---------- */
    free(primes);
    cache[r] = res;
    ready[r] = true;

    return res;
}
