/*
 * A243017  Least number k > 0 such that k*n/(k+n) and k*n/(k-n) are integers
 *          or 0 if no such k exists.
 * https://oeis.org/A243017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 878, u32 n <= 7144, u64 n <= 7144,
 *            u128 n <= 7144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal k such that k*n/(k+n) and k*n/(k-n) are integers
   (A243017).  The function computes the value for any n ≥ 1
   by enumerating all divisors of n².  If no such k exists, 0 is
   returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static size_t factor_uint64(uint64_t n, uint64_t *primes, int *exps)
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

static void dfs_divisors(const uint64_t *primes, const int *exps,
                         size_t cnt, size_t idx, seq_t cur,
                         seq_t sn, seq_t sn2, seq_t n_sq,
                         seq_t *best_k, int *found)
{
    if (idx == cnt) {
        if (cur > sn) {
            seq_t abs_e = (cur >= sn2)? cur - sn2 : sn2 - cur;
            if (abs_e != (seq_t)0 && (n_sq % abs_e) == (seq_t)0) {
                seq_t k = cur - sn;
                if (!*found || k < *best_k) {
                    *best_k = k;
                    *found = 1;
                }
            }
        }
        return;
    }
    seq_t val = cur;
    for (int e = 0; e <= 2 * exps[idx]; ++e) {
        dfs_divisors(primes, exps, cnt, idx + 1, val, sn, sn2,
                     n_sq, best_k, found);
        val *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;

    /* Factorise n */
    uint64_t primes[16];
    int exps[16];
    size_t np = factor_uint64((uint64_t)n, primes, exps);

    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * (seq_t)2;
    seq_t n_sq = sn * sn;

    seq_t best_k = (seq_t)0;
    int found = 0;

    dfs_divisors(primes, exps, np, 0, (seq_t)1, sn, sn2, n_sq,
                 &best_k, &found);

    return found ? best_k : (seq_t)0;
}
