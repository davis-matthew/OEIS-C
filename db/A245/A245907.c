/*
 * A245907  Let D = {d(n,i)}, i = 1..q, denote the set of divisors of n; then
 *          a(n) = number of multiplicative groups G(n,p) = D/kZ, 1 < k < n.
 * https://oeis.org/A245907
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the integers k (2 ≤ k < n) for which the set of
 *            residues of all divisors of n modulo k forms a multiplicative
 *            group. The function implements this definition for any n ≥ 2
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Euclidean gcd for idx_t (uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute a(n) according to the definition of A245907 */
seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;

    /* ---- compute all divisors of n ---- */
    idx_t divs[250];      /* enough for n up to >10^6 */
    size_t divcnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[divcnt++] = i;
            if (i != n / i)
                divs[divcnt++] = n / i;
        }
    }

    /* ---- reusable data structures for residues ---- */
    static uint32_t seen[20005];   /* mark residues by a tick counter */
    static uint32_t tick = 1;      /* increments for each new k */
    idx_t residues[256];           /* store unique residues for this k */

    seq_t count = 0;               /* result to return */

    for (idx_t k = 2; k < n; ++k) {
        tick++;                    /* new tick for this k */
        size_t rcnt = 0;
        bool valid = true;

        /* gather residues of divisors modulo k */
        for (size_t d = 0; d < divcnt; ++d) {
            idx_t res = divs[d] % k;
            if (res == 0) {        /* a residue of 0 invalidates the group */
                valid = false;
                break;
            }
            if (gcd_idx(res, k) != 1) {   /* not a unit mod k -> no inverse */
                valid = false;
                break;
            }
            if (seen[res] != tick) {
                seen[res] = tick;          /* mark presence */
                residues[rcnt++] = res;    /* keep list for closure test */
            }
        }

        if (!valid)
            continue;

        /* closure test: every product of residues must be present */
        for (size_t i = 0; i < rcnt && valid; ++i) {
            for (size_t j = 0; j < rcnt; ++j) {
                idx_t prod = (residues[i] * residues[j]) % k;
                if (seen[prod] != tick) {
                    valid = false;
                    break;
                }
            }
        }

        if (valid)
            count++;
    }

    return count;
}
