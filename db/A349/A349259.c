/*
 * A349259  Numbers where A349258 reaches a record value.
 * https://oeis.org/A349259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th smallest m(v) that is smaller than
 *            every m(w) with w > v, where m(v) = min{ k : A349258(k) = v }.
 *            Since A349258(k) = Sum_{p^e || k} (2^popcount(e) - 1), each
 *            prime p only needs exponents e = 2^j - 1, which add weight 2^j -
 *            1. m(v) is found by a knapsack DP over the first...
 * Range:     u8 none, u16 n <= 12, u32 n <= 20, u64 n <= 35, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 74 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A349259_V 640
#define A349259_VMAX 300
#define A349259_NP 120
#define A349259_KMAX 9

static seq_t *A349259_rec = 0;
static size_t A349259_cnt = 0;
static bool A349259_ready = false;

static void A349259_build(void)
{
    int V = A349259_V;
    seq_t *dp = mem_new(seq_t, V + 1);
    bool *inf = (bool *)malloc((V + 1) * sizeof(bool));
    seq_t *pw = mem_new(seq_t, A349259_KMAX + 1);
    int primes[A349259_NP];
    int np = 0;
    for (int c = 2; np < A349259_NP; c++) {
        bool ok = true;
        for (int i = 0; i < np; i++)
            if (c % primes[i] == 0) { ok = false; break; }
        if (ok) primes[np++] = c;
    }
    for (int v = 0; v <= V; v++) inf[v] = true;
    inf[0] = false;
    dp[0] = 1;
    for (int pi = 0; pi < np; pi++) {
        int p = primes[pi];
        /* pw[j] = p^(2^j - 1), for j = 1..maxk (only while it fits) */
        int maxk = 0;
        {
            seq_t cur = 1;
            int e = 0;
            bool over = false;
            for (int j = 1; j <= A349259_KMAX && !over; j++) {
                int target = (1 << j) - 1;
                if (target > V) break;
                while (e < target) {
                    seq_t nx = cur * (seq_t)p;
                    if (nx / (seq_t)p != cur) { over = true; break; }
                    cur = nx;
                    e++;
                }
                if (over) break;
                pw[j] = cur;
                maxk = j;
            }
        }
        for (int v = V; v >= 1; v--) {
            for (int j = 1; j <= maxk; j++) {
                int w = (1 << j) - 1;
                if (w > v) break;
                if (inf[v - w]) continue;
                seq_t base = dp[v - w];
                seq_t c = base * pw[j];
                if (c / pw[j] != base) continue;
                if (inf[v] || c < dp[v]) {
                    dp[v] = c;
                    inf[v] = false;
                }
            }
        }
    }
    A349259_rec = mem_new(seq_t, A349259_VMAX + 2);
    size_t cnt = 0;
    bool have = false;
    seq_t curmin = 0;
    for (int v = V; v >= 0; v--) {
        if (inf[v]) continue;
        if (!have || dp[v] < curmin) {
            if (v <= A349259_VMAX) A349259_rec[cnt++] = dp[v];
            curmin = dp[v];
            have = true;
        }
    }
    for (size_t i = 0; i < cnt / 2; i++) {
        seq_t t = A349259_rec[i];
        A349259_rec[i] = A349259_rec[cnt - 1 - i];
        A349259_rec[cnt - 1 - i] = t;
    }
    A349259_cnt = cnt;
    mem_free(dp);
    free(inf);
    mem_free(pw);
    A349259_ready = true;
}

seq_t a(idx_t n)
{
    if (!A349259_ready) A349259_build();
    if (n < 1 || (size_t)n > A349259_cnt) return (seq_t)0;
    return A349259_rec[n - 1];
}
