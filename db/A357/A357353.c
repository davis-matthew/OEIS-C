/*
 * A357353  Frobenius number of (n, n+1, n+prime(1), n+prime(2), n+prime(3),
 *          ...).
 * https://oeis.org/A357353
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the Frobenius number of the set
 *      { n, n+1, n+prime(1), n+prime(2), n+prime(3), … }
 *
 *  The algorithm runs Dijkstra’s shortest‑path on the residue ring
 *  ℤ/nℤ.  For each residue r the smallest coin with that residue is
 *  used.  The largest non‑representable number is max_r (s(r) – n),
 *  where s(r) is the minimal sum with residue r.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* offset starts at 2 */

    /* ----------------------------------------------------------- */
    /* 1.  Prime sieve up to a generous bound.
     *    Bound = n * 200 (≈ 8.5·10^5 for n = 10^4, safe for all cases)   */
    size_t bound = (size_t)n * 200;
    if (bound < 100)
        bound = 100;

    bool *sieve = (bool *)calloc(bound + 1, sizeof(bool));
    if (!sieve)
        return 0;                 /* allocation failure – return 0 */

    for (size_t i = 2; i <= bound; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= bound; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= bound; j += i)
                sieve[j] = false;

    /* ----------------------------------------------------------- */
    /* 2.  Find the minimal prime for every residue modulo n.
     *    We use 0 to denote “not found yet”.                         */
    size_t *min_prime = (size_t *)calloc((size_t)n, sizeof(size_t));
    if (!min_prime) {
        free(sieve);
        return 0;
    }

    for (size_t p = 2; p <= bound; ++p)
        if (sieve[p]) {
            size_t r = p % (size_t)n;
            if (r == 0)
                continue;          /* residue 0 never needed */
            if (min_prime[r] == 0 || p < min_prime[r])
                min_prime[r] = p;
        }

    /* ----------------------------------------------------------- */
    /* 3.  Build the list of coins (values) and their residues.
     *    For residue 1 we always have the coin n+1 (generator). */
    size_t max_coins = n;          /* at most one per residue (excluding 0) */
    seq_t *coin_val  = (seq_t *)calloc(max_coins, sizeof(seq_t));
    size_t *coin_res = (size_t *)calloc(max_coins, sizeof(size_t));
    if (!coin_val || !coin_res) {
        free(sieve); free(min_prime);
        free(coin_val); free(coin_res);
        return 0;
    }

    size_t coin_cnt = 0;
    /* residue 1 coin */
    coin_val[coin_cnt] = (seq_t)n + 1;
    coin_res[coin_cnt] = 1;
    ++coin_cnt;

    for (size_t r = 2; r < (size_t)n; ++r) {
        if (min_prime[r] != 0) {
            coin_val[coin_cnt] = (seq_t)n + (seq_t)min_prime[r];
            coin_res[coin_cnt] = r;
            ++coin_cnt;
        }
    }

    free(min_prime);            /* no longer needed */
    free(sieve);

    /* ----------------------------------------------------------- */
    /* 4.  Dijkstra on residues 0 .. n-1 */
    seq_t *dist   = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    bool *seen    = (bool *)calloc((size_t)n, sizeof(bool));
    bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
    if (!dist || !seen || !visited) {
        free(coin_val); free(coin_res);
        free(dist); free(seen); free(visited);
        return 0;
    }

    dist[0] = 0;
    seen[0] = true;              /* residue 0 is known with distance 0 */

    for (size_t step = 0; step < (size_t)n; ++step) {
        /* find unvisited residue with smallest known distance */
        seq_t best = 0;
        bool has_best = false;
        size_t u = 0;
        for (size_t i = 0; i < (size_t)n; ++i) {
            if (visited[i])
                continue;
            if (!seen[i])
                continue;
            if (!has_best || dist[i] < best) {
                best = dist[i];
                u = i;
                has_best = true;
            }
        }
        if (!has_best)            /* all remaining residues unreachable */
            break;
        visited[u] = true;

        /* relax all edges from u */
        for (size_t c = 0; c < coin_cnt; ++c) {
            size_t v = (u + coin_res[c]) % (size_t)n;
            seq_t nd = dist[u] + coin_val[c];
            if (!seen[v] || nd < dist[v]) {
                dist[v] = nd;
                seen[v] = true;
            }
        }
    }

    /* ----------------------------------------------------------- */
    /* 5.  Compute Frobenius number: max_r (dist[r] - n) , r != 0     */
    seq_t frob = 0;
    for (size_t r = 1; r < (size_t)n; ++r) {
        if (!seen[r])
            continue;              /* should not happen for valid n */
        seq_t cand = dist[r] - (seq_t)n;
        if (cand > frob)
            frob = cand;
    }

    /* ----------------------------------------------------------- */
    /* 6.  Clean up */
    free(coin_val); free(coin_res);
    free(dist);    free(seen); free(visited);

    return frob;
}
