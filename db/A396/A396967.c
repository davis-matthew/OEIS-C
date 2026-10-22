/*
 * A396967  Numbers k that are neither powers of primes nor have residues r
 *          (mod k) for which s = r + m*k is such that rad(s) | k but rad(r)
 *          does not divide k, and rad(k) does not divide r, where rad =
 *          A007947.
 * https://oeis.org/A396967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) is the n-th integer k that is not a prime power and has no
 *            residue r (mod k) such that there exists an integer m with s = r
 *            + m*k having rad(s) | k, while rad(r) does not divide k and
 *            rad(k) does not divide r. The function computes the sequence by
 *            iterating k, testing each with a BFS over re...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

/* --------------------------------------------------------------------- */
static bool _k_is_ok(uint64_t k)
{
    if (k < 2u) return false;          /* 1 is not in the sequence */

    /* Factor k; get distinct primes */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    /* collect distinct primes */
    uint64_t pk[32];
    size_t pcnt = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || primes[i] != primes[i-1])
            pk[pcnt++] = primes[i];
    }

    if (pcnt <= 1u) return false;  /* prime power -> exclude */

    /* BFS over residues of k‑smooth numbers modulo k */
    /* allocate visited and queue arrays on the heap */
    uint8_t *visited = (uint8_t*)malloc(k * sizeof(uint8_t));
    if (!visited) return false;    /* out of memory: treat as invalid */
    uint64_t *queue  = (uint64_t*)malloc(k * sizeof(uint64_t));
    if (!queue) { free(visited); return false; }

    memset(visited, 0, k * sizeof(uint8_t));
    size_t head = 0, tail = 0;

    uint64_t start = 1u % k;
    visited[start] = 1;
    queue[tail++] = start;

    while (head < tail) {
        uint64_t x = queue[head++];
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t y = (uint64_t)(((__uint128_t)x * pk[i]) % k);
            if (visited[y]) continue;
            visited[y] = 1;
            queue[tail++] = y;

            if (y == 0u) continue;  /* skip residue 0 */

            /* Check if y (the residue) has a prime outside pk */
            uint64_t tmp = y;
            for (size_t j = 0; j < pcnt; ++j)
                while (tmp % pk[j] == 0u) tmp /= pk[j];
            bool has_outside = (tmp > 1u);

            /* Check if y misses at least one prime from pk */
            bool missing = false;
            for (size_t j = 0; j < pcnt; ++j) {
                if (y % pk[j] != 0u) { missing = true; break; }
            }

            if (has_outside && missing) {
                free(visited);
                free(queue);
                return false;        /* bad residue found */
            }
        }
    }

    free(visited);
    free(queue);
    return true;                    /* no bad residue */
}

/* --------------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* Undefined for n=0, but return 0 */

    static seq_t *seq_vals = NULL;  /* cached sequence values */
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;
    static uint64_t checked = 1u;    /* largest k examined so far */

    /* grow cache as needed */
    if (seq_vals == NULL) {
        seq_cap = 64u;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_vals) return 0u;   /* memory error */
    }

    /* find terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++checked;
        if (_k_is_ok(checked)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2u;
                seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
                if (!seq_vals) return 0u;   /* memory error */
            }
            seq_vals[seq_len++] = (seq_t)checked;
        }
    }

    return seq_vals[n-1];
}
