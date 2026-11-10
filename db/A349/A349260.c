/*
 * A349260  a(n) is the least number k such that A349258(k) = n.
 * https://oeis.org/A349260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over n = sum_i (2^b_i - 1) of prod_i prime(i)^(2^b_i
 *            - 1); computed by a group knapsack over the primes, one exponent
 *            2^b-1 per prime
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 111 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    int N = (int)(long long)n;

    /* first N primes */
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 1));
    int pc = 0;
    for (uint64_t c = 2; pc < N; ++c) {
        bool ok = true;
        for (int i = 0; i < pc && pr[i] * pr[i] <= c; ++i)
            if (c % pr[i] == 0) { ok = false; break; }
        if (ok) pr[pc++] = c;
    }

    seq_t *dp = mem_new(seq_t, N + 1);
    seq_t *ndp = mem_new(seq_t, N + 1);
    unsigned char *valid = (unsigned char *)calloc((size_t)N + 1, 1);
    unsigned char *nvalid = (unsigned char *)calloc((size_t)N + 1, 1);
    dp[0] = (seq_t)1;
    valid[0] = 1;

    for (int i = 0; i < pc; ++i) {
        /* option b = 0: prime not used */
        for (int s = 0; s <= N; ++s) {
            ndp[s] = dp[s];
            nvalid[s] = valid[s];
        }
        seq_t pe = (seq_t)1;
        bool pe_ok = true;
        int cur_e = 0;
        for (int e = 1; e <= N && pe_ok; e = 2 * e + 1) {
            while (cur_e < e) {
                seq_t np = pe * (seq_t)pr[i];
                if (np / (seq_t)pr[i] != pe) { pe_ok = false; break; }
                pe = np;
                ++cur_e;
            }
            if (!pe_ok) break;
            for (int s = e; s <= N; ++s) {
                if (!valid[s - e]) continue;
                seq_t x = dp[s - e];
                seq_t r = x * pe;
                if (r / pe != x) continue;
                if (!nvalid[s] || r < ndp[s]) {
                    ndp[s] = r;
                    nvalid[s] = 1;
                }
            }
        }
        for (int s = 0; s <= N; ++s) {
            dp[s] = ndp[s];
            valid[s] = nvalid[s];
        }
    }

    seq_t res = dp[N];
    mem_free(dp);
    mem_free(ndp);
    free(valid);
    free(nvalid);
    free(pr);
    return res;
}
