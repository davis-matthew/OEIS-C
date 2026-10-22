/*
 * A395900  Number of residue classes r (mod n) for which k = r + m*n, m >= 1,
 *          is such that gcd(k,n) > 1 but rad(k) does not divide n, and rad(n)
 *          does not divide k, where rad = A007947.
 * https://oeis.org/A395900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of residues r (mod n) such that for all m ≥ 1, k =
 *            r + m*n satisfies gcd(k,n) > 1, rad(k) does not divide n, and
 *            rad(n) does not divide k. The algorithm enumerates the set of
 *            residues of n‑smooth numbers modulo n and cancels any r that (i)
 *            is coprime to n, (ii) is divisible by rad(n...
 * Range:     u8 n <= 478, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function follows exactly the OEIS specification:  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities supplied by the build infrastructure */
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 1U) return (seq_t)0U;

    /* ---- factor n to obtain its distinct prime factors ---- */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n, primes, exps, &cnt);

    /* ---- compute rad(n) = product of distinct prime factors ---- */
    idx_t rad_n = 1U;
    for (size_t i = 0; i < cnt; ++i)
        rad_n *= (idx_t)primes[i];

    /* ---- compute residues of n‑smooth numbers modulo n ---- */
    bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
    if (!visited) return (seq_t)0U;   /* allocation failure -> return 0 */

    /* Use a simple queue for BFS */
    size_t *queue = (size_t *)malloc((size_t)n * sizeof(size_t));
    if (!queue) { free(visited); return (seq_t)0U; }

    size_t qhead = 0, qtail = 0;
    visited[0U] = true;              /* residue 0 corresponds to 0 mod n, ignore but mark */
    visited[1U % (size_t)n] = true;
    queue[qtail++] = 1U % (size_t)n;

    while (qhead < qtail) {
        size_t cur = queue[qhead++];
        for (size_t i = 0; i < cnt; ++i) {
            size_t next = (cur * (size_t)(primes[i] % n)) % n;
            if (!visited[next]) {
                visited[next] = true;
                queue[qtail++] = next;
            }
        }
    }

    /* ---- count qualifying residues ---- */
    seq_t result = (seq_t)0U;
    for (idx_t r = 1U; r < n; ++r) {
        /* skip r that are coprime or divisible by rad(n) */
        if (gcd((seq_t)r, (seq_t)n) == (seq_t)1U) continue;
        if (r % rad_n == (idx_t)0U) continue;

        /* exclude residues that are n‑smooth (rad(k) | n) */
        if (!visited[(size_t)r]) ++result;
    }

    free(visited);
    free(queue);
    return result;
}
