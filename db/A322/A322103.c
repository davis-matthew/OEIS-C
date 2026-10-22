/*
 * A322103  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n} sigma_k(d).
 * https://oeis.org/A322103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array
 *            A(n,k)=∑_{d|n}σ_k(d) along anti‑diagonals. For given n, let s be
 *            the smallest integer with s*(s+1)/2 ≥ n. Then n = n_row +
 *            (s-1)*s/2, k = s – n_row, and a(n) = ∑_{d|n_row}σ_k(d)
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 175, u64 n <= 441,
 *            u128 n <= 1139, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_k.h"

static inline idx_t find_s(idx_t N) {
    if (N == 0) return 0;
    idx_t low = 1, high = 2;
    /* Find an upper bound */
    while (1) {
        __int128 pref = (__int128)high * (high + 1) / 2;
        if (pref >= N) break;
        high <<= 1;
        if (high == 0) {              /* overflow – stop at max value */
            high = (idx_t)-1;
            break;
        }
    }
    /* Binary search for the smallest s with s*(s+1)/2 ≥ N */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 pref = (__int128)mid * (mid + 1) / 2;
        if (pref >= N) high = mid;
        else           low = mid + 1;
    }
    return low;
}

static void dfs_div(const idx_t *primes, const int *exps, int cnt,
                    int pos, idx_t cur, uint8_t k, seq_t *sum) {
    if (pos == cnt) {
        *sum += sigma_k(cur, k);
        return;
    }
    idx_t p = primes[pos];
    idx_t val = 1;
    for (int e = 0; e <= exps[pos]; ++e) {
        dfs_div(primes, exps, cnt, pos + 1, cur * val, k, sum);
        val *= p;
    }
}

static seq_t compute_A(idx_t n, uint8_t k) {
    if (n == 0) return 0;
    idx_t tmp = n;
    idx_t primes[32];
    int exps[32];
    int cnt = 0;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }
    seq_t sum = 0;
    dfs_div(primes, exps, cnt, 0, 1, k, &sum);
    return sum;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t s = find_s(n);
    __int128 prev = (__int128)(s - 1) * s / 2;
    idx_t n_row = n - (idx_t)prev;
    uint8_t k_col = (uint8_t)(s - n_row);
    return compute_A(n_row, k_col);
}
