/*
 * A383180  Irregular table T(n,k) = A010846(A019565(2^n + k)).
 * https://oeis.org/A383180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A010846(A019565(n)) for n >= 0 (flat index of the table;
 *            row r>=1 has k=0..2^(r-1)-1 and uses A019565(2^(r-1)+k));
 *            a(0)=1. Counts m-smooth-over-prime-set numbers by DFS
 * Range:     u8 n <= 22, u16 n <= 509, u32 n <= 3961, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 80 terms through n = 9788 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint32_t T383180_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t T383180_ps[64];
static int T383180_cnt;
static seq_t T383180_m;

/* count numbers cur*(product of primes from index >= i) <= m, including cur itself */
static seq_t T383180_dfs(int i, seq_t cur)
{
    seq_t total = 1;
    for (int j = i; j < T383180_cnt; ++j) {
        seq_t p = T383180_ps[j];
        if (cur > T383180_m / p) continue;
        total += T383180_dfs(j, cur * p);
    }
    return total;
}

seq_t a(idx_t n)
{
    T383180_cnt = 0;
    seq_t m = 1;
    int bit = 0;
    idx_t t = n;
    while (t != 0) {
        if (t & 1ULL) {
            T383180_ps[T383180_cnt++] = (seq_t)T383180_primes[bit];
            m *= (seq_t)T383180_primes[bit];
        }
        t >>= 1;
        ++bit;
    }
    T383180_m = m;
    return T383180_dfs(0, (seq_t)1);
}
