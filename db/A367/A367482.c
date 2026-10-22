/*
 * A367482  Number of nontrivial divisors of n whose arithmetic derivative is
 *          coprime to n.
 * https://oeis.org/A367482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{d | n, d>1 and gcd(n, d') = 1}
 * Range:     u8 n <= 2056, u16 n <= 9969, u32 n <= 9969, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d' denotes the arithmetic derivative of d */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static void dfs_count(size_t pos, seq_t cur_val, uint64_t *cur_exp,
                      const uint64_t *p, const uint64_t *exp,
                      size_t pcnt, idx_t n, seq_t *cnt)
{
    if (pos == pcnt) {
        if (cur_val > (seq_t)1) {
            seq_t der = 0;
            for (size_t i = 0; i < pcnt; ++i) {
                if (cur_exp[i] == 0) continue;
                /* term = e_i * cur_val / p_i */
                der += (seq_t)cur_exp[i] * (cur_val / (seq_t)p[i]);
            }
            if (gcd(der, (seq_t)n) == 1) (*cnt)++;
        }
        return;
    }

    for (uint64_t e = 0; e <= exp[pos]; ++e) {
        cur_exp[pos] = e;
        seq_t next_val = cur_val;
        if (e > 0) {
            /* multiply by p[pos]^e */
            for (uint64_t k = 0; k < e; ++k)
                next_val *= (seq_t)p[pos];
        }
        dfs_count(pos + 1, next_val, cur_exp, p, exp,
                  pcnt, n, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    uint64_t cur_exp[20];
    size_t pcnt = 0;
    uint64_t tmp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            pcnt++;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        pcnt++;
    }

    seq_t count = 0;
    dfs_count(0, (seq_t)1, cur_exp, primes, exps,
              pcnt, n, &count);
    return count;
}
