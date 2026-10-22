/*
 * A390799  Number of ways to write n as (p-1)^4/16 + (q-1)^2/2 + u^2 + 3*v^2,
 *          where each of p and q is 1 or an odd prime, and u and v are in the
 *          set {m >= 0: 2*m is practical} with u > 0.
 * https://oeis.org/A390799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as (p-1)^4/16 + (q-1)^2/2 + u^2
 *            + 3*v^2 where p,q ∈ {1}∪{odd primes}, u>0 with 2*u practical,
 *            v≥0 with 2*v practical, offset: 1 (a(1) is the first listed
 *            term)
 * Range:     u8 n <= 100, u16 n <= 3395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_practical_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_practical_local.h"

/* integer square root for seq_t values */
static inline int isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq = mid * mid;
        if (sq == x) return (int)mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return (int)high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    const idx_t n_val = n;

    /* bounds for variables */
    int max_t = 0;
    for (int t = 0;; ++t) {
        seq_t t4 = (seq_t)t * t * t * t;
        if (t4 > n_val) break;
        max_t = t;
    }
    int max_s = 0;
    for (int s = 0;; ++s) {
        seq_t B = (seq_t)2 * s * s;
        if (B > n_val) break;
        max_s = s;
    }
    int max_u = 0;
    for (int u = 1;; ++u) {
        seq_t u2 = (seq_t)u * u;
        if (u2 > n_val) break;
        max_u = u;
    }
    int max_v = 0;
    for (int v = 0;; ++v) {
        seq_t B = (seq_t)3 * v * v;
        if (B > n_val) break;
        max_v = v;
    }

    /* prime sieve up to needed limit */
    int prime_limit = 2 * (max_t > max_s ? max_t : max_s) + 1;
    if (prime_limit < 3) prime_limit = 3;
    bool is_prime[2005];
    for (int i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= prime_limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= prime_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* t and s values (including 0) */
    int t_vals[25];
    int t_cnt = 0;
    int s_vals[100];
    int s_cnt = 0;
    for (int t = 0; t <= max_t; ++t) {
        int p = 2 * t + 1;
        if (t == 0 || (p <= prime_limit && is_prime[p]))
            t_vals[t_cnt++] = t;
    }
    for (int s = 0; s <= max_s; ++s) {
        int q = 2 * s + 1;
        if (s == 0 || (q <= prime_limit && is_prime[q]))
            s_vals[s_cnt++] = s;
    }

    /* u values (>0) and v validity array */
    int u_vals[200];
    int u_cnt = 0;
    bool v_valid[200];
    for (int i = 0; i < 200; ++i) v_valid[i] = false;
    v_valid[0] = true; /* v = 0 allowed */
    for (int u = 1; u <= max_u; ++u) {
        if (is_practical_local(2ull * (idx_t)u))
            u_vals[u_cnt++] = u;
    }
    for (int v = 1; v <= max_v; ++v) {
        if (is_practical_local(2ull * (idx_t)v))
            v_valid[v] = true;
    }

    seq_t count = 0;
    for (int ti = 0; ti < t_cnt; ++ti) {
        int t = t_vals[ti];
        seq_t t4 = (seq_t)t * t * t * t;
        if (t4 > n_val) break;
        for (int si = 0; si < s_cnt; ++si) {
            int s = s_vals[si];
            seq_t B = (seq_t)2 * s * s;
            seq_t rest = n_val - t4 - B;
            if (rest < 0) continue;
            for (int ui = 0; ui < u_cnt; ++ui) {
                int u = u_vals[ui];
                seq_t u2 = (seq_t)u * u;
                if (u2 > rest) break;
                seq_t rem = rest - u2;
                if (rem % 3u != 0u) continue;
                seq_t v2 = rem / 3u;
                int v = isqrt_seq(v2);
                if ((seq_t)v * v == v2 && v <= max_v && v_valid[v])
                    ++count;
            }
        }
    }
    return count;
}
